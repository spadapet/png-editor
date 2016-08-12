#include "pch.h"
#include "Core/Designer.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFileVM.h"
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

	if (this->expanded)
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
	for (auto i = newItems.begin(); i != newItems.end(); i++)
	{
#if 0
		if (old != this->items->Size)
		{
			IProjectItemVM ^oldItem = this->items->GetAt(old);
			if (*i != oldItem->It)
			{
				if (std::find(i + 1, newItems.end(), oldItem) == newItems.end())
				{
					*old = this->MakeItem(*i);
				}
				else
				{
					old = this->items.insert(old, this->MakeItem(*i));
				}
			}
		}
		else
		{
			old = this->items.insert(old, this->MakeItem(*i));
		}
#endif
	}

	// this->items.erase(old, this->items.end());
}
