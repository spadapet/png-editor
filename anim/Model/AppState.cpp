#include "pch.h"
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
	std::weak_ptr<AppState> weakThis = this->shared_from_this();

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Files,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		if (owner == nullptr)
		{
			return nullptr;
		}

		return ref new FilesPane(owner);
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Color,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		if (owner == nullptr)
		{
			return nullptr;
		}

		return ref new FilesPane(owner);
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Layers,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		if (owner == nullptr)
		{
			return nullptr;
		}

		return ref new FilesPane(owner);
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::View,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		if (owner == nullptr)
		{
			return nullptr;
		}

		return ref new FilesPane(owner);
	}));

	this->panes.emplace_back(std::make_shared<PaneInfo>(PaneType::Animation,
		[weakThis](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		auto owner = weakThis.lock();
		if (owner == nullptr)
		{
			return nullptr;
		}

		return ref new FilesPane(owner);
	}));
}

void anim::AppState::InitializeForDesigner()
{
	AppState::AssertDesigner();

	this->Initialize();
}

void anim::AppState::Load()
{
}

void anim::AppState::Save()
{
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
