#include "pch.h"
#include "Core/Command.h"
#include "Core/String.h"
#include "Core/Thread.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "Model/RootProjectFolder.h"
#include "View/Utility/FlatProjectItems.h"
#include "ViewModel/ProjectFileVM.h"
#include "ViewModel/ProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

static Windows::UI::Xaml::Input::ICommand ^activateCommand = nullptr;
static Windows::UI::Xaml::Input::ICommand ^deleteCommand = nullptr;

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder, ProjectFolderVM ^parent)
	: folder(folder)
	, weakParent(parent != nullptr ? Platform::WeakReference(parent) : Platform::WeakReference(nullptr))
	, items(ref new Platform::Collections::Vector<IProjectItemVM ^>())
	, flatItems(ref new FlatProjectItems(this->items))
	, expanded(false)
{
	Platform::WeakReference weakOwner(this);

	this->folderChangedCookie = this->folder->PropertyChanged.Add([weakOwner](const char *name)
	{
		ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
		if (owner != nullptr)
		{
			owner->FolderPropertyChanged(name);
		}
	});

	// Root folders always start off expanded
	if (this->Level == 0)
	{
		anim::PostToMainThread([weakOwner]()
		{
			ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
			if (owner != nullptr)
			{
				owner->ShowExpanded = true;
			}
		});
	}
}

anim::ProjectFolderVM::ProjectFolderVM()
	: ProjectFolderVM(std::make_shared<RootProjectFolder>(Windows::Storage::ApplicationData::Current->TemporaryFolder, AppState::CreateForDesigner()), nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
	this->folder->PropertyChanged.Remove(this->folderChangedCookie);
}

std::shared_ptr<anim::ProjectFolder> anim::ProjectFolderVM::Model::get()
{
	return this->folder;
}

Windows::Storage::IStorageItem ^anim::ProjectFolderVM::Item::get()
{
	return this->folder->GetItem();
}

Platform::String ^anim::ProjectFolderVM::DisplayName::get()
{
	if (this->Level == 0)
	{
		Windows::Storage::StorageFolder ^folder = this->folder->GetFolder();
		std::wostringstream str;
		str << folder->Name->Data() << L" (" << folder->Path->Data() << L")";
		return ref new Platform::String(str.str().c_str());
	}

	return this->folder->GetFolder()->Name;
}

Platform::String ^anim::ProjectFolderVM::FullPath::get()
{
	return this->folder->GetFolder()->Path;
}

int anim::ProjectFolderVM::Level::get()
{
	return this->folder->GetLevel();
}

anim::ProjectFileVM ^anim::ProjectFolderVM::AsFile::get()
{
	return nullptr;
}

anim::ProjectFolderVM ^anim::ProjectFolderVM::AsFolder::get()
{
	return this;
}

anim::IProjectItemVM ^anim::ProjectFolderVM::Parent::get()
{
	return this->weakParent.Resolve<IProjectItemVM>();
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectFolderVM::ActivateCommand::get()
{
	if (::activateCommand == nullptr)
	{
		::activateCommand = ref new anim::Command([](Platform::Object ^item)
		{
			ProjectFolderVM ^folder = dynamic_cast<ProjectFolderVM ^>(item);
			if (folder != nullptr && folder->HasItems)
			{
				folder->ShowExpanded = !folder->ShowExpanded;
			}
		});
	}

	return ::activateCommand;
}

Windows::Storage::StorageFolder ^anim::ProjectFolderVM::Folder::get()
{
	return this->folder->GetFolder();
}

Windows::Foundation::Collections::IVector<anim::IProjectItemVM ^> ^anim::ProjectFolderVM::Items::get()
{
	return this->items;
}

Windows::UI::Xaml::Interop::IBindableObservableVector ^anim::ProjectFolderVM::BindableItems::get()
{
	return this->items;
}

Windows::UI::Xaml::Interop::IBindableObservableVector ^anim::ProjectFolderVM::BindableFlatItems::get()
{
	return this->flatItems->BindableItems;
}

bool anim::ProjectFolderVM::HasItems::get()
{
	return this->folder->HasItems();
}

bool anim::ProjectFolderVM::ShowExpanded::get()
{
	return this->expanded;
}

void anim::ProjectFolderVM::ShowExpanded::set(bool value)
{
	if (this->expanded != value)
	{
		this->expanded = value;
		this->NotifyPropertyChanged("ShowExpanded");
		this->NotifyPropertyChanged("ShowExpandedIcon");

		if (this->expanded)
		{
			this->UpdateItems();
		}
	}
}

bool anim::ProjectFolderVM::ShowExpandedIcon::get()
{
	return this->ShowExpanded && this->HasItems;
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectFolderVM::DeleteCommand::get()
{
	if (::deleteCommand == nullptr)
	{
		::deleteCommand = ref new anim::Command([](Platform::Object ^item)
		{
			ProjectFolderVM ^folder = dynamic_cast<ProjectFolderVM ^>(item);
			if (folder != nullptr)
			{
				if (folder->Level == 0)
				{
					std::shared_ptr<AppState> app = folder->folder->GetAppState();
					if (app != nullptr)
					{
						app->RemoveProjectFolder(folder->Folder);
					}
				}
				else
				{
				}
			}
		});
	}

	return ::deleteCommand;
}

void anim::ProjectFolderVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ProjectFolderVM::FolderPropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);

	if (allChanged || strcmp(name, "Items") == 0)
	{
		this->UpdateItems();
	}

	if (allChanged || strcmp(name, "HasItems") == 0)
	{
		this->NotifyPropertyChanged("HasItems");
		this->NotifyPropertyChanged("ShowExpandedIcon");
	}
}

void anim::ProjectFolderVM::UpdateItems()
{
	const std::vector<std::shared_ptr<ProjectItem>> &newItems = this->folder->GetItems();

	unsigned int old = 0;
	for (auto newItem = newItems.begin(); newItem != newItems.end(); newItem++, old++)
	{
		if (old != this->items->Size)
		{
			IProjectItemVM ^oldItem = this->items->GetAt(old);
			if ((*newItem)->GetItem() != oldItem->Item)
			{
				if (std::find_if(newItem + 1, newItems.end(),
					[&oldItem](const std::shared_ptr<ProjectItem> &newItem2)
					{
						return newItem2->GetItem() == oldItem->Item;
					}) == newItems.end())
				{
					this->items->SetAt(old, this->MakeVM(*newItem));
				}
				else
				{
					this->items->InsertAt(old, this->MakeVM(*newItem));
				}
			}
		}
		else
		{
			this->items->Append(this->MakeVM(*newItem));
		}
	}

	while (old < this->items->Size)
	{
		this->items->RemoveAtEnd();
	}
}

anim::IProjectItemVM ^anim::ProjectFolderVM::MakeVM(std::shared_ptr<ProjectItem> item)
{
	if (item->IsFolder())
	{
		std::shared_ptr<ProjectFolder> folder = std::dynamic_pointer_cast<ProjectFolder>(item);
		return ref new ProjectFolderVM(folder, this);
	}

	if (item->IsFile())
	{
		std::shared_ptr<ProjectFile> file = std::dynamic_pointer_cast<ProjectFile>(item);
		return ref new ProjectFileVM(file, this);
	}

	return ref new ProjectItemVM(item, this);
}
