#include "pch.h"
#include "Core/String.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "Model/PaneInfo.h"
#include "Model/ProjectFile.h"
#include "Model/ProjectFolder.h"
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
		if (project->Equals(folder))
		{
			return;
		}
	}

	std::shared_ptr<ProjectFolder> project = std::dynamic_pointer_cast<ProjectFolder>(this->RegisterProjectItem(nullptr, folder));
	if (project != nullptr)
	{
		this->projectFolders.push_back(project);
		this->ProjectFolderAdded.Notify(project);
	}
}

void anim::AppState::RemoveProjectFolder(Windows::Storage::StorageFolder ^folder)
{
	for (auto i = this->projectFolders.begin(); i != this->projectFolders.end(); i++)
	{
		std::shared_ptr<ProjectFolder> project = *i;

		if (project->Equals(folder))
		{
			this->projectFolders.erase(i);
			this->ProjectFolderRemoved.Notify(project);
			break;
		}
	}
}

std::shared_ptr<anim::OpenFile> anim::AppState::EditFile(std::shared_ptr<ProjectFile> file)
{
	return file->GetOpenFile();
}

void anim::AppState::CloseFile(std::shared_ptr<OpenFile> file)
{
	for (std::shared_ptr<OpenFile> openFile : this->openFiles)
	{
		if (file == openFile)
		{
		}
	}
}

std::shared_ptr<anim::ProjectItem> anim::AppState::RegisterProjectItem(std::shared_ptr<ProjectItem> parent, Windows::Storage::IStorageItem ^item)
{
	std::string name = anim::ConvertString(item->Path);
	std::shared_ptr<ProjectItem> model;

	auto i = this->itemCache.find(name);
	if (i != this->itemCache.end())
	{
		model = i->second.lock();
		if (model == nullptr)
		{
			this->itemCache.erase(i);
		}
		else if (model->GetParent() == nullptr && parent != nullptr)
		{
			model->SetParent(parent);
		}
	}

	if (model == nullptr)
	{
		if (item->IsOfType(Windows::Storage::StorageItemTypes::Folder))
		{
			if (parent == nullptr)
			{
				model = std::make_shared<ProjectFolder>(safe_cast<Windows::Storage::StorageFolder ^>(item), this->shared_from_this());
			}
			else
			{
				model = std::make_shared<ProjectFolder>(safe_cast<Windows::Storage::StorageFolder ^>(item), parent);
			}
		}
		else if (item->IsOfType(Windows::Storage::StorageItemTypes::File))
		{
			if (parent == nullptr)
			{
				model = std::make_shared<ProjectFile>(safe_cast<Windows::Storage::StorageFile ^>(item), this->shared_from_this());
			}
			else
			{
				model = std::make_shared<ProjectFile>(safe_cast<Windows::Storage::StorageFile ^>(item), parent);
			}
		}
		else if (parent == nullptr)
		{
			model = std::make_shared<ProjectItem>(item, this->shared_from_this());
		}
		else
		{
			model = std::make_shared<ProjectItem>(item, parent);
		}

		this->itemCache.emplace(std::make_pair(std::move(name), std::weak_ptr<ProjectItem>(model)));
	}

	return model;
}

void anim::AppState::UnregisterProjectItem(Windows::Storage::IStorageItem ^item)
{
	std::string name = anim::ConvertString(item->Path);
	auto i = this->itemCache.find(name);

	if (i != this->itemCache.end())
	{
		this->itemCache.erase(i);
	}
}

void anim::AppState::PurgeExpiredProjectItems()
{
	for (auto i = this->itemCache.begin(); i != this->itemCache.end(); )
	{
		if (i->second.expired())
		{
			i = this->itemCache.erase(i);
		}
		else
		{
			i++;
		}
	}
}
