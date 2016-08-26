#include "pch.h"
#include "Core/Thread.h"
#include "Model/ProjectFile.h"
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

void anim::ProjectFolder::SetItem(Windows::Storage::IStorageItem ^item)
{
	Windows::Storage::StorageFolder ^folder = dynamic_cast<Windows::Storage::StorageFolder ^>(item);
	assert(folder != nullptr);

	if (folder != nullptr)
	{
		this->SetFolder(folder);
	}
}

bool anim::ProjectFolder::IsFolder() const
{
	return true;
}

void anim::ProjectFolder::SetFolder(Windows::Storage::StorageFolder ^folder)
{
	if (folder != this->folder)
	{
		this->folder = folder;
		ProjectItem::SetItem(folder);
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
	return !this->initQuery || !this->items.empty();
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

	this->PropertyChanged.Notify("HasItems");

	this->Refresh();
}

void anim::ProjectFolder::Refresh()
{
	anim::AssertMainThread();

	if (this->query == nullptr)
	{
		return;
	}

	std::weak_ptr<ProjectItem> weakOwner = this->shared_from_this();

	auto getTask = concurrency::create_task(this->query->GetItemsAsync());

	getTask.then([weakOwner](Windows::Foundation::Collections::IVectorView<Windows::Storage::IStorageItem ^> ^items)
	{
		std::shared_ptr<ProjectFolder> owner = std::dynamic_pointer_cast<ProjectFolder>(weakOwner.lock());
		if (owner != nullptr)
		{
			owner->Merge(std::vector<Windows::Storage::IStorageItem ^>(begin(items), end(items)));
		}
	}, concurrency::task_continuation_context::use_current());
}

void anim::ProjectFolder::Merge(std::vector<Windows::Storage::IStorageItem ^> newItems)
{
	anim::AssertMainThread();

	bool changed = false;
	auto old = this->items.begin();
	for (auto i = newItems.begin(); i != newItems.end(); i++, old++)
	{
		if (old != this->items.end())
		{
			Windows::Storage::IStorageItem ^oldItem = (*old)->GetItem();
			if (*i != oldItem)
			{
				if (std::find(i + 1, newItems.end(), oldItem) == newItems.end())
				{
					*old = this->MakeItem(*i);
					changed = true;
				}
				else
				{
					old = this->items.insert(old, this->MakeItem(*i));
					changed = true;
				}
			}
		}
		else
		{
			old = this->items.insert(old, this->MakeItem(*i));
			changed = true;
		}
	}

	if (old != this->items.end())
	{
		this->items.erase(old, this->items.end());
		changed = true;
	}

	if (changed)
	{
		this->PropertyChanged.Notify("HasItems");
		this->PropertyChanged.Notify("Items");
	}
}

std::shared_ptr<anim::ProjectItem> anim::ProjectFolder::MakeItem(Windows::Storage::IStorageItem ^item)
{
	std::shared_ptr<ProjectFolder> owner = std::dynamic_pointer_cast<ProjectFolder>(this->shared_from_this());

	if (item->IsOfType(Windows::Storage::StorageItemTypes::Folder))
	{
		return std::make_shared<ProjectFolder>(safe_cast<Windows::Storage::StorageFolder ^>(item), owner);
	}
	else if (item->IsOfType(Windows::Storage::StorageItemTypes::File))
	{
		return std::make_shared<ProjectFile>(safe_cast<Windows::Storage::StorageFile ^>(item), owner);
	}

	return std::make_shared<ProjectItem>(item, owner);
}
