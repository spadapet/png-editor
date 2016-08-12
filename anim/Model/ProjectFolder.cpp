#include "pch.h"
#include "Core/Thread.h"
#include "Model/ProjectFolder.h"

static Windows::Storage::Search::QueryOptions ^GetQueryOptions()
{
	static Windows::Storage::Search::QueryOptions ^options = nullptr;
	if (options == nullptr)
	{
		Platform::Collections::Vector<Platform::String ^> ^fileFilter = ref new Platform::Collections::Vector<Platform::String ^>
		{
			".png",
		};

		options = ref new Windows::Storage::Search::QueryOptions(Windows::Storage::Search::CommonFileQuery::DefaultQuery, fileFilter);
	}

	return options;
}

anim::ProjectFolder::ProjectFolder(Windows::Storage::StorageFolder ^folder, std::shared_ptr<ProjectFolder> parent)
	: ProjectItem(folder, parent)
	, folder(folder)
	, initQuery(false)
{
}

anim::ProjectFolder::~ProjectFolder()
{
	if (this->query != nullptr)
	{
		this->query->ContentsChanged -= this->queryChangedToken;
	}
}

Windows::Storage::StorageFolder ^anim::ProjectFolder::GetFolder() const
{
	return this->folder;
}

void anim::ProjectFolder::SetFolder(Windows::Storage::StorageFolder ^folder)
{
	if (folder != this->folder)
	{
		this->folder = folder;
		this->SetItem(folder);
		this->PropertyChanged.Notify("Folder");
	}
}

const std::vector<std::shared_ptr<anim::ProjectItem>> &anim::ProjectFolder::GetItems()
{
	this->InitializeQuery();

	return this->items;
}

bool anim::ProjectFolder::HasItems() const
{
	return !this->items.empty();
}

bool anim::ProjectFolder::CheckedForItems() const
{
	return this->initQuery;
}

void anim::ProjectFolder::InitializeQuery()
{
	if (this->initQuery)
	{
		return;
	}

	this->initQuery = true;

	if (this->folder->AreQueryOptionsSupported(::GetQueryOptions()))
	{
		std::weak_ptr<ProjectItem> weakOwner = this->shared_from_this();

		this->query = this->folder->CreateItemQueryWithOptions(::GetQueryOptions());

		this->queryChangedToken = this->query->ContentsChanged +=
			ref new Windows::Foundation::TypedEventHandler<Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^>
				([weakOwner](Windows::Storage::Search::IStorageQueryResultBase ^, Platform::Object ^)
		{
			anim::RunOnMainThread([weakOwner]()
			{
				std::shared_ptr<ProjectFolder> owner = std::dynamic_pointer_cast<ProjectFolder>(weakOwner.lock());
				if (owner != nullptr)
				{
					owner->Refresh();
				}
			});
		});
	}

	this->PropertyChanged.Notify("CheckedForItems");
}

void anim::ProjectFolder::Refresh()
{
#if 0
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
#endif

	this->PropertyChanged.Notify("HasItems");
	this->PropertyChanged.Notify("Items");
}

void anim::ProjectFolder::Merge(std::vector<Windows::Storage::IStorageItem ^> newItems)
{
	anim::AssertMainThread();

#if 0
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
					this->folders->SetAt(curOld, ref new ProjectFolderVM(std::make_shared<ProjectFolder>(*i, this->folder)));
				}
				else
				{
					this->folders->InsertAt(curOld, ref new ProjectFolderVM(std::make_shared<ProjectFolder>(*i, this->folder)));
				}
			}
		}
		else
		{
			this->folders->Append(ref new ProjectFolderVM(std::make_shared<ProjectFolder>(*i, this->folder)));
		}
	}

	for (; curOld < this->folders->Size; curOld++)
	{
		this->folders->RemoveAt(curOld);
	}
#endif

	this->PropertyChanged.Notify("HasItems");
}
