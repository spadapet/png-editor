#include "pch.h"
#include "Controller/AppPersist.h"
#include "Core/String.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"

static Windows::Storage::AccessCache::StorageItemAccessList ^GetFutureAccessList()
{
	return Windows::Storage::AccessCache::StorageApplicationPermissions::FutureAccessList;
}

Windows::Storage::StorageFolder ^GetCacheFolder()
{
	return Windows::Storage::ApplicationData::Current->LocalCacheFolder;
}

static Platform::String ^GetCacheFileName()
{
	return "AppStateCached.json";
}

static void LoadAppState(std::shared_ptr<anim::AppState> app, Windows::Data::Json::JsonObject ^root)
{
	std::vector<Platform::String ^> projectFolderTokens;
	if (root->HasKey("ProjectFolderTokens"))
	{
		Windows::Data::Json::JsonArray ^tokens = root->GetNamedArray("ProjectFolderTokens");
		for (unsigned int i = 0; i < tokens->Size; i++)
		{
			Platform::String ^token = tokens->GetStringAt(i);
			projectFolderTokens.push_back(token);
		}
	}

	for (Platform::String ^token : projectFolderTokens)
	{
		auto getTask = concurrency::create_task(
			::GetFutureAccessList()->GetFolderAsync(token,
				Windows::Storage::AccessCache::AccessCacheOptions::DisallowUserInput));

		getTask.then([app](Windows::Storage::StorageFolder ^folder)
		{
			anim::PostToMainThread([app, folder]()
			{
				app->AddProjectFolder(folder);
			});
		});
	}
}

concurrency::task<std::shared_ptr<anim::AppState>> anim::LoadAppState()
{
	std::shared_ptr<AppState> app = AppState::Create();

	auto fileTask = concurrency::create_task(::GetCacheFolder()->GetFileAsync(::GetCacheFileName()));

	auto openTask = fileTask.then([](Windows::Storage::StorageFile ^file)
	{
		return file->OpenReadAsync();
	});

	auto readTask = openTask.then([](Windows::Storage::Streams::IRandomAccessStreamWithContentType ^stream)
	{
		Windows::Storage::Streams::IBuffer ^buffer = ref new Windows::Storage::Streams::Buffer((unsigned int)stream->Size);
		Windows::Storage::Streams::IInputStream ^input = stream->GetInputStreamAt(0);

		return input->ReadAsync(buffer, (unsigned int)stream->Size, Windows::Storage::Streams::InputStreamOptions::None);
	});

	auto parseTask = readTask.then([](Windows::Storage::Streams::IBuffer ^buffer) -> Windows::Data::Json::JsonObject ^
	{
		Windows::Storage::Streams::DataReader ^reader = Windows::Storage::Streams::DataReader::FromBuffer(buffer);
		Platform::Array<unsigned char> ^bytes = ref new Platform::Array<unsigned char>(buffer->Length);

		reader->ReadBytes(bytes);
		std::string jsonText((const char *)bytes->Data, bytes->Length);

		Windows::Data::Json::JsonObject ^root;
		return Windows::Data::Json::JsonObject::Parse(anim::ConvertString(jsonText));
	});

	auto doneTask = parseTask.then([app](concurrency::task<Windows::Data::Json::JsonObject ^> parseTask)
	{
		try
		{
			Windows::Data::Json::JsonObject ^root = parseTask.get();
			::LoadAppState(app, root);
		}
		catch (Platform::Exception ^ex)
		{
			// doesn't matter if state can't be loaded
		}

		return app;
	});

	return doneTask;
}

concurrency::task<void> anim::SaveAppState(std::shared_ptr<AppState> app)
{
	anim::AssertMainThread();

	std::shared_ptr<std::string> text = std::make_shared<std::string>();
	std::vector<Windows::Storage::StorageFolder ^> projectFolders;

	for (std::shared_ptr<ProjectFolder> folder : app->GetProjectFolders())
	{
		projectFolders.push_back(folder->GetFolder());
	}

	auto stringTask = concurrency::create_task([projectFolders, text]()
	{
		Windows::Data::Json::JsonObject ^root = ref new Windows::Data::Json::JsonObject();
		Windows::Data::Json::JsonArray ^tokens = ref new Windows::Data::Json::JsonArray();
		root->SetNamedValue("ProjectFolderTokens", tokens);

		::GetFutureAccessList()->Clear();

		for (Windows::Storage::StorageFolder ^folder : projectFolders)
		{
			Platform::String ^token = ::GetFutureAccessList()->Add(folder);
			Windows::Data::Json::JsonValue ^value = Windows::Data::Json::JsonValue::CreateStringValue(token);
			tokens->Append(value);
		}

		*text = anim::ConvertString(root->Stringify());
	});

	auto createTask = stringTask.then([]()
	{
		return ::GetCacheFolder()->CreateFileAsync(::GetCacheFileName(), Windows::Storage::CreationCollisionOption::ReplaceExisting);
	});

	auto openTask = createTask.then([](Windows::Storage::StorageFile ^file)
	{
		return file->OpenAsync(Windows::Storage::FileAccessMode::ReadWrite);
	});

	auto saveTask = openTask.then([text](Windows::Storage::Streams::IRandomAccessStream ^stream)
	{
		Windows::Storage::Streams::DataWriter ^writer = ref new Windows::Storage::Streams::DataWriter();
		Platform::ArrayReference<unsigned char> bytes((unsigned char *)text->c_str(), (unsigned int)text->size());
		writer->WriteBytes(bytes);

		Windows::Storage::Streams::IOutputStream ^output = stream->GetOutputStreamAt(0);
		output->WriteAsync(writer->DetachBuffer());
	});

	auto doneTask = saveTask.then([](concurrency::task<void> task)
	{
		try
		{
			task.get();
		}
		catch (Platform::Exception ^ex)
		{
			// doesn't matter if state can't be saved
		}
	});

	return doneTask;
}

