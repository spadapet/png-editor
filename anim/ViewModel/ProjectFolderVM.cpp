#include "pch.h"
#include "Core/Designer.h"
#include "Model/ProjectFile.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFileVM.h"
#include "ViewModel/ProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder)
	: folder(folder)
	, items(ref new Platform::Collections::Vector<IProjectItemVM ^>())
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
}

anim::ProjectFolderVM::ProjectFolderVM()
	: ProjectFolderVM(std::make_shared<ProjectFolder>(Windows::Storage::ApplicationData::Current->TemporaryFolder, nullptr))
{
	anim::AssertXamlDesigner();
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
	this->folder->PropertyChanged.Remove(this->folderChangedCookie);
}

Windows::Storage::IStorageItem ^anim::ProjectFolderVM::Item::get()
{
	return this->folder->GetItem();
}

Platform::String ^anim::ProjectFolderVM::DisplayName::get()
{
	return this->folder->GetFolder()->DisplayName;
}

Platform::String ^anim::ProjectFolderVM::FullPath::get()
{
	return this->folder->GetFolder()->Path;
}

int anim::ProjectFolderVM::Level::get()
{
	return this->folder->GetLevel();
}

Windows::Storage::StorageFolder ^anim::ProjectFolderVM::Folder::get()
{
	return this->folder->GetFolder();
}

Windows::Foundation::Collections::IVector<anim::IProjectItemVM ^> ^anim::ProjectFolderVM::Items::get()
{
	return this->items;
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
	}

	if (this->expanded && !this->folder->CheckedForItems())
	{
		this->UpdateItems();
	}
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
					[oldItem](const std::shared_ptr<ProjectItem> &newItem2)
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
		return ref new ProjectFolderVM(folder);
	}

	if (item->IsFile())
	{
		std::shared_ptr<ProjectFile> file = std::dynamic_pointer_cast<ProjectFile>(item);
		return ref new ProjectFileVM(file);
	}

	return ref new ProjectItemVM(item);
}
