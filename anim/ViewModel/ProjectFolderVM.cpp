#include "pch.h"
#include "Core/Designer.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFolderVM.h"

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder)
	: folder(folder)
	, folders(ref new Platform::Collections::Vector<ProjectFolderVM ^>())
	, files(ref new Platform::Collections::Vector<ProjectFileVM ^>())
{
	this->Refresh();
}

anim::ProjectFolderVM::ProjectFolderVM()
	: ProjectFolderVM(nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
}

Windows::Storage::StorageFolder ^anim::ProjectFolderVM::Folder::get()
{
	return this->folder->GetFolder();
}

Platform::String ^anim::ProjectFolderVM::DisplayName::get()
{
	return this->folder->GetFolder()->DisplayName;
}

Platform::String ^anim::ProjectFolderVM::FullPath::get()
{
	return this->folder->GetFolder()->Path;
}

Windows::Foundation::Collections::IVector<anim::ProjectFolderVM ^> ^anim::ProjectFolderVM::Folders::get()
{
	return this->folders;
}

Windows::Foundation::Collections::IVector<anim::ProjectFileVM ^> ^anim::ProjectFolderVM::Files::get()
{
	return this->files;
}

void anim::ProjectFolderVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ProjectFolderVM::Refresh()
{
	Platform::WeakReference weakOwner(this);

	this->folders->Clear();
	this->files->Clear();

	auto getTask = concurrency::create_task(this->folder->GetFolder()->GetItemsAsync());

	getTask.then([weakOwner](Windows::Foundation::Collections::IVectorView<Windows::Storage::IStorageItem ^> ^items)
	{
		ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
		if (owner == nullptr)
		{
			return;
		}

		for (Windows::Storage::IStorageItem ^item : items)
		{
			if (item->IsOfType(Windows::Storage::StorageItemTypes::Folder))
			{
				Windows::Storage::StorageFolder ^folder = dynamic_cast<Windows::Storage::StorageFolder ^>(item);
				if (folder != nullptr)
				{
				}
			}
			else if (item->IsOfType(Windows::Storage::StorageItemTypes::File))
			{
				Windows::Storage::StorageFile ^file = dynamic_cast<Windows::Storage::StorageFile ^>(item);
				if (file != nullptr)
				{
				}
			}
		}

	}, concurrency::task_continuation_context::use_current());
}
