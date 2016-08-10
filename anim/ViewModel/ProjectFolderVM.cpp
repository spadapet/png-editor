#include "pch.h"
#include "Core/Designer.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFolderVM.h"

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder)
	: folder(folder)
	, folders(ref new Platform::Collections::Vector<ProjectFolderVM ^>())
	, files(ref new Platform::Collections::Vector<ProjectFileVM ^>())
{
	if (this->folder != nullptr && this->folder->GetFolder()->IsCommonFolderQuerySupported(Windows::Storage::Search::CommonFolderQuery::DefaultQuery))
	{
		this->folderQuery = this->folder->GetFolder()->CreateFolderQuery(Windows::Storage::Search::CommonFolderQuery::DefaultQuery);
	}

	if (this->folder != nullptr && this->folder->GetFolder()->IsCommonFileQuerySupported(Windows::Storage::Search::CommonFileQuery::OrderByName))
	{
		Platform::Collections::Vector<Platform::String ^> ^fileFilter = ref new Platform::Collections::Vector<Platform::String ^>
		{
			".png"
		};

		Windows::Storage::Search::QueryOptions ^options = ref new Windows::Storage::Search::QueryOptions(Windows::Storage::Search::CommonFileQuery::OrderByName, fileFilter);
		this->fileQuery = this->folder->GetFolder()->CreateFileQueryWithOptions(options);
	}

	this->RefreshFolders();
	this->RefreshFiles();

#ifdef _DEBUG
	if (this->folder != nullptr)
	{
		::OutputDebugString(Platform::String::Concat(this->folder->GetFolder()->Path, "\r\n")->Data());
	}
#endif
}

anim::ProjectFolderVM::ProjectFolderVM()
	: ProjectFolderVM(nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
	if (this->folderQuery != nullptr && this->folderChangedToken.Value != 0)
	{
		this->folderQuery->ContentsChanged -= this->folderChangedToken;
	}

	if (this->fileQuery != nullptr && this->fileChangedToken.Value != 0)
	{
		this->fileQuery->ContentsChanged -= this->fileChangedToken;
	}
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

void anim::ProjectFolderVM::RefreshFolders()
{
	if (this->folderQuery == nullptr)
	{
		return;
	}

	auto getTask = concurrency::create_task(this->folderQuery->GetFoldersAsync());

	Platform::WeakReference weakOwner(this);
	getTask.then([weakOwner](Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFolder ^> ^items)
	{
		ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
		if (owner == nullptr)
		{
			return;
		}

		owner->folders->Clear();

		for (Windows::Storage::StorageFolder ^item : items)
		{
			std::shared_ptr<ProjectFolder> folderModel = std::make_shared<ProjectFolder>(item);
			ProjectFolderVM ^folderVM = ref new ProjectFolderVM(folderModel);
			owner->folders->Append(folderVM);
		}

		if (owner->folderChangedToken.Value != 0)
		{
			return;
		}

		owner->folderChangedToken = owner->folderQuery->ContentsChanged +=
			ref new Windows::Foundation::TypedEventHandler<Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^>
			([weakOwner](Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^)
		{
			anim::PostToMainThread([weakOwner]()
			{
				ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
				if (owner != nullptr)
				{
					owner->RefreshFolders();
				}
			});
		});
	}, concurrency::task_continuation_context::use_current());
}

void anim::ProjectFolderVM::RefreshFiles()
{
	if (this->fileQuery == nullptr)
	{
		return;
	}

	auto getTask = concurrency::create_task(this->fileQuery->GetFilesAsync());

	Platform::WeakReference weakOwner(this);
	getTask.then([weakOwner](Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile ^> ^items)
	{
		ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
		if (owner == nullptr)
		{
			return;
		}

		owner->files->Clear();

		for (Windows::Storage::StorageFile ^item : items)
		{
			std::shared_ptr<ProjectFile> fileModel = std::make_shared<ProjectFile>(item);
			ProjectFileVM ^fileVM = ref new ProjectFileVM(fileModel);
			owner->files->Append(fileVM);
		}

		if (owner->fileChangedToken.Value != 0)
		{
			return;
		}

		owner->fileChangedToken = owner->fileQuery->ContentsChanged +=
			ref new Windows::Foundation::TypedEventHandler<Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^>
			([weakOwner](Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^)
		{
			anim::PostToMainThread([weakOwner]()
			{
				ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
				if (owner != nullptr)
				{
					owner->RefreshFiles();
				}
			});
		});
	}, concurrency::task_continuation_context::use_current());
}
