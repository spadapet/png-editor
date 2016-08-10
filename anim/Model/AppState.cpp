#include "pch.h"
#include "Core/Designer.h"
#include "Core/String.h"
#include "Model/AppState.h"
#include "Model/PaneInfo.h"
#include "Model/RootProjectFolder.h"
#include "View/FilesPane.xaml.h"

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

std::shared_ptr<anim::AppState> anim::AppState::Create()
{
	std::shared_ptr<AppState> app = std::make_shared<AppState>();
	app->Initialize();
	return app;
}

std::shared_ptr<anim::AppState> anim::AppState::CreateForDesigner()
{
	std::shared_ptr<AppState> app = std::make_shared<AppState>();
	app->InitializeForDesigner();
	return app;
}

void anim::AppState::Initialize()
{
	this->ResetPanes();
}

void anim::AppState::InitializeForDesigner()
{
	anim::AssertXamlDesigner();

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
	if (folder == nullptr)
	{
		return;
	}

	for (std::shared_ptr<ProjectFolder> project : this->projectFolders)
	{
		if (project->GetFolder() == folder)
		{
			return;
		}
	}

	std::shared_ptr<ProjectFolder> project = std::make_shared<RootProjectFolder>(folder);
	this->projectFolders.push_back(project);
	this->ProjectFolderAdded.Notify(project);
}

void anim::AppState::RemoveProjectFolder(Windows::Storage::StorageFolder ^folder)
{
	for (auto i = this->projectFolders.begin(); i != this->projectFolders.end(); i++)
	{
		std::shared_ptr<ProjectFolder> project = *i;

		if (project->GetFolder() == folder)
		{
			this->projectFolders.erase(i);
			this->ProjectFolderRemoved.Notify(project);
			break;
		}
	}
}
