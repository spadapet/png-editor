#include "pch.h"
#include "Core/String.h"
#include "Model/AppState.h"
#include "Model/PaneInfo.h"
#include "Model/ProjectFolder.h"
#include "UI/FilesPane.xaml.h"

static Windows::UI::Xaml::UIElement ^CreateNonePane(anim::PaneType type) 
{
	return nullptr;
}

anim::AppState::AppState()
	: nonePane(std::make_shared<PaneInfo>(PaneType::None, &::CreateNonePane))
{
}

anim::AppState::~AppState()
{
}

std::shared_ptr<anim::AppState> anim::AppState::CreateForDesigner()
{
	std::shared_ptr<AppState> app = std::make_shared<AppState>();
	app->InitializeForDesigner();
	return app;
}

void anim::AppState::AssertDesigner()
{
	assert(AppState::IsDesigner());
}

bool anim::AppState::IsDesigner()
{
	return Windows::ApplicationModel::DesignMode::DesignModeEnabled;
}

void anim::AppState::Initialize()
{
	this->ResetPanes();
}

void anim::AppState::InitializeForDesigner()
{
	AppState::AssertDesigner();

	this->Initialize();
}

void anim::AppState::ResetPanes()
{
	std::weak_ptr<AppState> weakThis = this->shared_from_this();

	this->panes.clear();

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Files,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		std::shared_ptr<AppState> owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Color,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		std::shared_ptr<AppState> owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Layers,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		std::shared_ptr<AppState> owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::View,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		std::shared_ptr<AppState> owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Animation,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		std::shared_ptr<AppState> owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->PropertyChanged.Notify("Panes");
}

concurrency::task<void> anim::AppState::Load()
{
	this->Initialize();

	Windows::Storage::StorageFolder ^folder = Windows::Storage::ApplicationData::Current->LocalCacheFolder;

	std::shared_ptr<AppState> owner = this->shared_from_this();
	auto fileTask = concurrency::create_task(folder->GetFileAsync("AppStateCached.json"));
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
		if (!Windows::Data::Json::JsonObject::TryParse(anim::ConvertString(jsonText), &root))
		{
			root = nullptr;
		}

		return root;
	});

	auto doneTask = parseTask.then([owner](concurrency::task<Windows::Data::Json::JsonObject ^> parseTask)
	{
		try
		{
			Windows::Data::Json::JsonObject ^root = parseTask.get();

			if (root != nullptr)
			{
				owner->Load(root);
			}
		}
		catch (Platform::Exception ^ex)
		{
			// doesn't matter if state can't be loaded
		}
	}, concurrency::task_continuation_context::use_current());

	return doneTask;
}

void anim::AppState::Load(Windows::Data::Json::JsonObject ^root)
{
	std::shared_ptr<AppState> owner = this->shared_from_this();
	std::vector<Platform::String ^> projectFolderTokens;

	Windows::Storage::AccessCache::StorageItemAccessList ^futureAccessList =
		Windows::Storage::AccessCache::StorageApplicationPermissions::FutureAccessList;

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
			futureAccessList->GetFolderAsync(token, Windows::Storage::AccessCache::AccessCacheOptions::DisallowUserInput));

		getTask.then([owner](Windows::Storage::StorageFolder ^folder)
		{
			if (folder != nullptr)
			{
				owner->AddProjectFolder(folder);
			}
		}, concurrency::task_continuation_context::use_current());
	}
}

concurrency::task<void> anim::AppState::Save()
{
	std::vector<Windows::Storage::StorageFolder ^> projectFolders;
	for (std::shared_ptr<ProjectFolder> folder : this->projectFolders)
	{
		projectFolders.push_back(folder->GetFolder());
	}

	auto createTask = concurrency::create_task([projectFolders]()
	{
		Windows::Data::Json::JsonObject ^root = ref new Windows::Data::Json::JsonObject();
		Windows::Data::Json::JsonArray ^tokens = ref new Windows::Data::Json::JsonArray();
		root->SetNamedValue("ProjectFolderTokens", tokens);

		Windows::Storage::AccessCache::StorageItemAccessList ^futureAccessList =
			Windows::Storage::AccessCache::StorageApplicationPermissions::FutureAccessList;
		futureAccessList->Clear();

		for (Windows::Storage::StorageFolder ^folder : projectFolders)
		{
			Platform::String ^token = futureAccessList->Add(folder);
			Windows::Data::Json::JsonValue ^value = Windows::Data::Json::JsonValue::CreateStringValue(token);
			tokens->Append(value);
		}

		return root;
	});

	auto saveTask = createTask.then([](Windows::Data::Json::JsonObject ^root)
	{
	});

	auto doneTask = saveTask.then([](concurrency::task<void> task)
	{
		try
		{
			task.get();
		}
		catch (Platform::Exception ^ex)
		{
		}
	});

	return doneTask;
}

const std::vector<std::shared_ptr<anim::PaneInfo>> &anim::AppState::GetPanes() const
{
	return this->panes;
}

const std::vector<std::shared_ptr<anim::ProjectFolder>> &anim::AppState::GetProjectFolders() const
{
	return this->projectFolders;
}

std::shared_ptr<anim::PaneInfo> anim::AppState::GetNonePane() const
{
	return this->nonePane;
}

anim::AppMode anim::AppState::GetMode() const
{
	return AppMode::None;
}

void anim::AppState::AddProjectFolder(Windows::Storage::StorageFolder ^folder)
{
}
