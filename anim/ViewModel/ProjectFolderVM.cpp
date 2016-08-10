#include "pch.h"
#include "Core/Designer.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFolderVM.h"

static Windows::Storage::Search::QueryOptions ^GetFileQueryOptions()
{
	static Windows::Storage::Search::QueryOptions ^options;

	if (options == nullptr)
	{
		Platform::Collections::Vector<Platform::String ^> ^fileFilter = ref new Platform::Collections::Vector<Platform::String ^>
		{
			".png"
		};

		options = ref new Windows::Storage::Search::QueryOptions(Windows::Storage::Search::CommonFileQuery::DefaultQuery, fileFilter);
	}

	return options;
}

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder)
	: folder(folder)
	, folders(ref new Platform::Collections::Vector<ProjectFolderVM ^>())
	, files(ref new Platform::Collections::Vector<ProjectFileVM ^>())
	, expanded(false)
{
	Platform::WeakReference weakOwner(this);

	if (this->folder->GetFolder()->IsCommonFolderQuerySupported(Windows::Storage::Search::CommonFolderQuery::DefaultQuery))
	{
		this->folderQuery = this->folder->GetFolder()->CreateFolderQuery(Windows::Storage::Search::CommonFolderQuery::DefaultQuery);

		this->folderChangedToken = this->folderQuery->ContentsChanged +=
			ref new Windows::Foundation::TypedEventHandler<Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^>
			([weakOwner](Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^)
		{
			anim::RunOnMainThread([weakOwner]()
			{
				ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
				if (owner != nullptr)
				{
					owner->RefreshFolders();
				}
			});
		});
	}

	if (this->folder->GetFolder()->AreQueryOptionsSupported(::GetFileQueryOptions()))
	{
		this->fileQuery = this->folder->GetFolder()->CreateFileQueryWithOptions(::GetFileQueryOptions());

		this->fileChangedToken = this->fileQuery->ContentsChanged +=
			ref new Windows::Foundation::TypedEventHandler<Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^>
			([weakOwner](Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^)
		{
			anim::RunOnMainThread([weakOwner]()
			{
				ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
				if (owner != nullptr)
				{
					owner->RefreshFiles();
				}
			});
		});
	}

#ifdef _DEBUG
	::OutputDebugString(Platform::String::Concat(this->FullPath, "\r\n")->Data());
#endif
}

anim::ProjectFolderVM::ProjectFolderVM(Windows::Storage::StorageFolder ^folder)
	: ProjectFolderVM(std::make_shared<ProjectFolder>(folder))
{
}

anim::ProjectFolderVM::ProjectFolderVM()
	: ProjectFolderVM(Windows::Storage::ApplicationData::Current->TemporaryFolder)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
	if (this->folderQuery != nullptr)
	{
		this->folderQuery->ContentsChanged -= this->folderChangedToken;
	}

	if (this->fileQuery != nullptr)
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

bool anim::ProjectFolderVM::HasChildren::get()
{
	return this->folders->Size > 0 || this->files->Size > 0;
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
	if (this->folderQuery != nullptr)
	{
		Platform::WeakReference weakOwner(this);
		auto getTask = concurrency::create_task(this->folderQuery->GetFoldersAsync());

		getTask.then([weakOwner](Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFolder ^> ^items)
		{
			ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
			if (owner != nullptr)
			{
				owner->MergeFolders(std::vector<Windows::Storage::StorageFolder ^>(begin(items), end(items)));
			}
		}, concurrency::task_continuation_context::use_current());
	}
}

void anim::ProjectFolderVM::RefreshFiles()
{
	if (this->fileQuery != nullptr)
	{
		Platform::WeakReference weakOwner(this);
		auto getTask = concurrency::create_task(this->fileQuery->GetFilesAsync());

		getTask.then([weakOwner](Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile ^> ^items)
		{
			anim::PostToMainThread([weakOwner, items]()
			{
				ProjectFolderVM ^owner = weakOwner.Resolve<ProjectFolderVM>();
				if (owner != nullptr)
				{
					owner->MergeFiles(std::vector<Windows::Storage::StorageFile ^>(begin(items), end(items)));
				}
			});
		});
	}
}

void anim::ProjectFolderVM::MergeFolders(std::vector<Windows::Storage::StorageFolder ^> newFolders)
{
	anim::AssertMainThread();

	std::sort(newFolders.begin(), newFolders.end(),
		[](Windows::Storage::StorageFolder ^lhs, Windows::Storage::StorageFolder ^rhs)
	{
		return ::_wcsicmp(lhs->DisplayName->Data(), rhs->DisplayName->Data()) < 0;
	});

	unsigned int curOld = 0;
	for (auto i = newFolders.begin(); i != newFolders.end(); i++, curOld++)
	{
		if (curOld < this->folders->Size)
		{
			Windows::Storage::StorageFolder ^oldFolder = this->folders->GetAt(curOld)->Folder;
			if (*i != oldFolder)
			{
				if (std::find(i + 1, newFolders.end(), oldFolder) == newFolders.end())
				{
					this->folders->SetAt(curOld, ref new ProjectFolderVM(*i));
				}
				else
				{
					this->folders->InsertAt(curOld, ref new ProjectFolderVM(*i));
				}
			}
		}
		else
		{
			this->folders->Append(ref new ProjectFolderVM(*i));
		}
	}

	for (; curOld < this->folders->Size; curOld++)
	{
		this->folders->RemoveAt(curOld);
	}

	this->NotifyPropertyChanged("HasChildren");
}

void anim::ProjectFolderVM::MergeFiles(std::vector<Windows::Storage::StorageFile ^> newFiles)
{
	anim::AssertMainThread();

	std::sort(newFiles.begin(), newFiles.end(),
		[](Windows::Storage::StorageFile ^lhs, Windows::Storage::StorageFile ^rhs)
	{
		return ::_wcsicmp(lhs->DisplayName->Data(), rhs->DisplayName->Data()) < 0;
	});

	unsigned int curOld = 0;
	for (auto i = newFiles.begin(); i != newFiles.end(); i++, curOld++)
	{
		if (curOld < this->files->Size)
		{
			Windows::Storage::StorageFile ^oldFile = this->files->GetAt(curOld)->File;
			if (*i != oldFile)
			{
				if (std::find(i + 1, newFiles.end(), oldFile) == newFiles.end())
				{
					this->files->SetAt(curOld, ref new ProjectFileVM(*i));
				}
				else
				{
					this->files->InsertAt(curOld, ref new ProjectFileVM(*i));
				}
			}
		}
		else
		{
			this->files->Append(ref new ProjectFileVM(*i));
		}
	}

	for (; curOld < this->files->Size; curOld++)
	{
		this->files->RemoveAt(curOld);
	}

	this->NotifyPropertyChanged("HasChildren");
}
