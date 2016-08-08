#include "pch.h"
#include "Core/String.h"
#include "Model/PaneInfo.h"
#include "Model/AppState.h"
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
	auto app = std::make_shared<AppState>();
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
		auto owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Color,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Layers,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::View,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Animation,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		return (owner != nullptr) ? ref new FilesPane(owner) : nullptr;
	}));

	this->PropertyChanged.Notify("Panes");
}

concurrency::task<void> anim::AppState::Load()
{
	auto owner = this->shared_from_this();

	this->Initialize();

	Windows::Storage::StorageFolder ^folder = Windows::Storage::ApplicationData::Current->LocalCacheFolder;
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

	return readTask.then([owner](concurrency::task<Windows::Storage::Streams::IBuffer ^> bufferTask)
	{
		try
		{
			Windows::Storage::Streams::IBuffer ^buffer = bufferTask.get();
			Windows::Storage::Streams::DataReader ^reader = Windows::Storage::Streams::DataReader::FromBuffer(buffer);
			Platform::Array<unsigned char> ^bytes = ref new Platform::Array<unsigned char>(buffer->Length);

			reader->ReadBytes(bytes);
			std::string jsonText((const char *)bytes->Data, bytes->Length);

			Windows::Data::Json::JsonObject ^root;
			if (Windows::Data::Json::JsonObject::TryParse(anim::ConvertString(jsonText), &root))
			{
				owner->Load(root);
			}
		}
		catch (Platform::Exception ^ex)
		{
		}
	});
}

void anim::AppState::Load(Windows::Data::Json::JsonObject ^root)
{
	if (root->HasKey("ProjectFolderTokens"))
	{
		Windows::Data::Json::JsonArray ^tokens = root->GetNamedArray("ProjectFolderTokens");
		for (unsigned int i = 0; i < tokens->Size; i++)
		{
			Platform::String ^token = tokens->GetStringAt(i);
			this->projectFolderTokens.push_back(anim::ConvertString(token));
		}
	}
}

void anim::AppState::Save()
{
	Windows::Data::Json::JsonObject ^root = ref new Windows::Data::Json::JsonObject();
}

const std::vector<std::shared_ptr<anim::PaneInfo>> &anim::AppState::GetPanes() const
{
	return this->panes;
}

const std::vector<Windows::Storage::StorageFolder ^> &anim::AppState::GetProjectFolders() const
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
