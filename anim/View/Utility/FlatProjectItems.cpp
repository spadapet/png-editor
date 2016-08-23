#include "pch.h"
#include "Model/AppState.h"
#include "View/Utility/FlatProjectItems.h"
#include "ViewModel/IProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

anim::FlatProjectItems::FlatProjectItems()
	: FlatProjectItems(ref new Platform::Collections::Vector<IProjectItemVM ^>())
{
}

anim::FlatProjectItems::FlatProjectItems(Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^rootItems)
	: rootItems(rootItems)
	, size(0)
	, blockNotifications(1)
	, cachedIndex(0)
{
	this->rootItemsChangedCookie = this->rootItems->VectorChanged +=
		ref new Windows::Foundation::Collections::VectorChangedEventHandler<IProjectItemVM ^>(
			this, &FlatProjectItems::OnRootItemsChanged);

	this->ResetRootEntries();
	this->blockNotifications--;
}

anim::FlatProjectItems::~FlatProjectItems()
{
	this->rootItems->VectorChanged -= this->rootItemsChangedCookie;
}

Windows::UI::Xaml::Interop::IBindableObservableVector ^anim::FlatProjectItems::BindableItems::get()
{
	return this;
}

Windows::UI::Xaml::Interop::IBindableIterator ^anim::FlatProjectItems::First()
{
	return ref new FlatProjectItems::Iterator(this->rootItems);
}

unsigned int anim::FlatProjectItems::Size::get()
{
	return this->size;
}

Platform::Object ^anim::FlatProjectItems::GetAt(unsigned int index)
{
	if (this->cachedItem != nullptr && index == this->cachedIndex)
	{
		return this->cachedItem;
	}

	unsigned int cur = 0;
	for (IProjectItemVM ^item : this->rootItems)
	{
		if (cur == index)
		{
			return item;
		}

		cur++;

		ProjectFolderVM ^folder = item->AsFolder;
		if (folder != nullptr && folder->ShowExpanded)
		{
			Windows::UI::Xaml::Interop::IBindableVector ^folderItems = folder->BindableFlatItems;
			unsigned int nestedCount = folderItems->Size;

			if (index - cur < nestedCount)
			{
				return folderItems->GetAt(index - cur);
			}

			cur += nestedCount;
		}
	}

	return nullptr;
}

Windows::UI::Xaml::Interop::IBindableVectorView ^anim::FlatProjectItems::GetView()
{
	std::vector<Platform::Object ^> view;
	view.reserve(this->Size);

	for (Windows::UI::Xaml::Interop::IBindableIterator ^iter = this->First(); iter->HasCurrent; iter->MoveNext())
	{
		view.push_back(iter->Current);
	}

	return ref new Platform::Collections::VectorView<Platform::Object ^>(std::move(view));
}

bool anim::FlatProjectItems::IndexOf(Platform::Object ^value, unsigned int *index)
{
	unsigned int cur = 0;
	for (IProjectItemVM ^item : this->rootItems)
	{
		if (item == value)
		{
			if (index != nullptr)
			{
				*index = cur;
			}

			return true;
		}

		cur++;

		ProjectFolderVM ^folder = item->AsFolder;
		if (folder != nullptr && folder->ShowExpanded)
		{
			Windows::UI::Xaml::Interop::IBindableVector ^folderItems = folder->BindableFlatItems;
			unsigned int nestedCount = folderItems->Size;

			if (folder->BindableFlatItems->IndexOf(value, index))
			{
				if (index != nullptr)
				{
					*index += cur;
				}

				return true;
			}

			cur += nestedCount;
		}
	}

	return false;
}

void anim::FlatProjectItems::SetAt(unsigned int index, Platform::Object ^value)
{
	throw ref new Platform::NotImplementedException();
}

void anim::FlatProjectItems::InsertAt(unsigned int index, Platform::Object ^value)
{
	throw ref new Platform::NotImplementedException();
}

void anim::FlatProjectItems::RemoveAt(unsigned int index)
{
	throw ref new Platform::NotImplementedException();
}

void anim::FlatProjectItems::Append(Platform::Object ^value)
{
	throw ref new Platform::NotImplementedException();
}

void anim::FlatProjectItems::RemoveAtEnd()
{
	throw ref new Platform::NotImplementedException();
}

void anim::FlatProjectItems::Clear()
{
	throw ref new Platform::NotImplementedException();
}

void anim::FlatProjectItems::OnRootItemsChanged(
	Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^sender,
	Windows::Foundation::Collections::IVectorChangedEventArgs ^args)
{
	switch (args->CollectionChange)
	{
	case Windows::Foundation::Collections::CollectionChange::ItemChanged:
		this->RemoveRootEntry(args->Index);
		this->AddRootEntry(this->rootItems->GetAt(args->Index), args->Index);
		break;

	case Windows::Foundation::Collections::CollectionChange::ItemInserted:
		this->AddRootEntry(this->rootItems->GetAt(args->Index), args->Index);
		break;

	case Windows::Foundation::Collections::CollectionChange::ItemRemoved:
		this->RemoveRootEntry(args->Index);
		break;

	case Windows::Foundation::Collections::CollectionChange::Reset:
		this->ResetRootEntries();
		break;
	}
}

void anim::FlatProjectItems::OnRootFolderChanged(ProjectFolderVM ^folder, Platform::String ^name)
{
	bool allChanged = (name == nullptr || name->Length() == 0);

	if (allChanged || name == "ShowExpanded")
	{
		if (folder->ShowExpanded)
		{
			this->AddRootFolderChildren(folder);
		}
		else
		{
			this->RemoveRootFolderChildren(folder);
		}
	}
}

void anim::FlatProjectItems::OnRootFolderItemsChanged(ProjectFolderVM ^folder, Windows::Foundation::Collections::IVectorChangedEventArgs ^args)
{
	if (!folder->ShowExpanded)
	{
		return;
	}

	switch (args->CollectionChange)
	{
	case Windows::Foundation::Collections::CollectionChange::ItemChanged:
	case Windows::Foundation::Collections::CollectionChange::ItemInserted:
	case Windows::Foundation::Collections::CollectionChange::ItemRemoved:
		{
			unsigned int flatIndex = this->FlatIndexOfRootFolder(folder);
			IProjectItemVM ^item = nullptr;

			if (args->CollectionChange == Windows::Foundation::Collections::CollectionChange::ItemInserted)
			{
				item = dynamic_cast<IProjectItemVM ^>(folder->BindableFlatItems->GetAt(args->Index));
				this->size++;
			}
			else if (args->CollectionChange == Windows::Foundation::Collections::CollectionChange::ItemRemoved)
			{
				this->size--;
			}

			this->NotifyVectorChanged(args->CollectionChange, flatIndex + 1 + args->Index, item);
		}
		break;

	case Windows::Foundation::Collections::CollectionChange::Reset:
		this->RemoveRootFolderChildren(folder, true);
		this->AddRootFolderChildren(folder);
		break;
	}
}

void anim::FlatProjectItems::ResetRootEntries()
{
	this->blockNotifications++;
	this->entries.clear();
	this->size = 0;

	for (IProjectItemVM ^item : this->rootItems)
	{
		this->AddRootEntry(item);
	}

	this->blockNotifications--;
	this->NotifyVectorChanged(Windows::Foundation::Collections::CollectionChange::Reset);
}

void anim::FlatProjectItems::AddRootEntry(IProjectItemVM ^item, unsigned int index)
{
	if (index == INVALID_UINT)
	{
		index = (unsigned int)this->entries.size();
	}

	this->size++;
	auto iter = this->entries.emplace(this->entries.begin() + index);

	ProjectFolderVM ^folder = item->AsFolder;
	iter->folder = folder;

	unsigned int flatIndex = this->FlatIndexOfRootFolder(folder);
	this->NotifyVectorChanged(Windows::Foundation::Collections::CollectionChange::ItemInserted, flatIndex, folder);

	if (folder != nullptr)
	{
		Platform::WeakReference weakOwner(this);

		iter->folderChangedCookie = folder->PropertyChanged +=
			ref new Windows::UI::Xaml::Data::PropertyChangedEventHandler(
				[weakOwner, folder](Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args)
		{
			FlatProjectItems ^owner = weakOwner.Resolve<FlatProjectItems>();
			if (owner != nullptr)
			{
				owner->OnRootFolderChanged(folder, args->PropertyName);
			}
		});

		iter->itemsChangedCookie = folder->BindableFlatItems->VectorChanged +=
			ref new Windows::UI::Xaml::Interop::BindableVectorChangedEventHandler(
				[weakOwner, folder](Windows::UI::Xaml::Interop::IBindableObservableVector ^items, Platform::Object ^argsObject)
		{
			FlatProjectItems ^owner = weakOwner.Resolve<FlatProjectItems>();
			Windows::Foundation::Collections::IVectorChangedEventArgs ^args = dynamic_cast<Windows::Foundation::Collections::IVectorChangedEventArgs ^>(argsObject);

			if (owner != nullptr && args != nullptr)
			{
				owner->OnRootFolderItemsChanged(folder, args);
			}
		});

		if (folder->ShowExpanded)
		{
			this->AddRootFolderChildren(folder);
		}
	}
}

void anim::FlatProjectItems::RemoveRootEntry(unsigned int index)
{
	auto iter = this->entries.begin() + index;
	if (iter->folder != nullptr)
	{
		this->RemoveRootFolderChildren(iter->folder);
	}

	unsigned int flatIndex = this->FlatIndexOfRootIndex(index);
	this->entries.erase(iter);
	this->size--;
	this->NotifyVectorChanged(Windows::Foundation::Collections::CollectionChange::ItemRemoved, flatIndex);
}

void anim::FlatProjectItems::AddRootFolderChildren(ProjectFolderVM ^folder, bool force)
{
	unsigned int realIndex;
	unsigned int flatIndex = this->FlatIndexOfRootFolder(folder, &realIndex);
	Entry &entry = this->entries[realIndex];
	assert(entry.folder == folder);

	if (force || (!entry.addedChildren && folder->ShowExpanded))
	{
		unsigned int size = folder->BindableFlatItems->Size;
		this->size += size;
		entry.addedChildren = true;

		unsigned int cur = 0;
		for (Windows::UI::Xaml::Interop::IBindableIterator ^iter = folder->BindableFlatItems->First(); iter->HasCurrent; iter->MoveNext())
		{
			this->NotifyVectorChanged(
				Windows::Foundation::Collections::CollectionChange::ItemInserted,
				flatIndex + 1 + cur,
				dynamic_cast<IProjectItemVM ^>(iter->Current));
			cur++;
		}
	}
}

void anim::FlatProjectItems::RemoveRootFolderChildren(ProjectFolderVM ^folder, bool force)
{
	unsigned int realIndex;
	unsigned int flatIndex = this->FlatIndexOfRootFolder(folder, &realIndex);
	Entry &entry = this->entries[realIndex];
	assert(entry.folder == folder);

	if (force || (entry.addedChildren && !folder->ShowExpanded))
	{
		unsigned int size = folder->BindableFlatItems->Size;
		this->size -= size;
		entry.addedChildren = false;

		for (size_t i = 0; i < size; i++)
		{
			this->NotifyVectorChanged(Windows::Foundation::Collections::CollectionChange::ItemRemoved, flatIndex + 1);
		}
	}
}

void anim::FlatProjectItems::NotifyVectorChanged(
	Windows::Foundation::Collections::CollectionChange change,
	unsigned int index,
	IProjectItemVM ^item)
{
	if (this->blockNotifications == 0)
	{
		if (change == Windows::Foundation::Collections::CollectionChange::ItemInserted)
		{
			this->cachedIndex = index;
			this->cachedItem = item;
		}

		this->VectorChanged(this, ref new Platform::Collections::Details::VectorChangedEventArgs(change, index));

		if (change == Windows::Foundation::Collections::CollectionChange::ItemInserted)
		{
			this->cachedIndex = 0;
			this->cachedItem = nullptr;
		}
	}
}

unsigned int anim::FlatProjectItems::FlatIndexOfRootFolder(ProjectFolderVM ^folder, unsigned int *realIndex)
{
	if (realIndex != nullptr)
	{
		*realIndex = 0;
	}

	unsigned int flatIndex = 0;
	for (IProjectItemVM ^rootItem : this->rootItems)
	{
		ProjectFolderVM ^otherFolder = rootItem->AsFolder;
		if (otherFolder == folder)
		{
			break;
		}

		flatIndex++;

		if (realIndex != nullptr)
		{
			++*realIndex;
		}

		if (otherFolder != nullptr && otherFolder->ShowExpanded)
		{
			flatIndex += otherFolder->BindableFlatItems->Size;
		}
	}

	return flatIndex;
}

unsigned int anim::FlatProjectItems::FlatIndexOfRootIndex(unsigned int index)
{
	unsigned int flatIndex = 0;
	for (unsigned int i = 0; i < index; i++, flatIndex++)
	{
		const Entry &entry = this->entries[i];
		if (entry.folder != nullptr && entry.folder->ShowExpanded)
		{
			flatIndex += entry.folder->BindableFlatItems->Size;
		}
	}

	return flatIndex;
}

anim::FlatProjectItems::Entry::Entry()
	: addedChildren(false)
{
}

anim::FlatProjectItems::Entry::Entry(Entry &&rhs)
	: folder(rhs.folder)
	, addedChildren(rhs.addedChildren)
	, folderChangedCookie(rhs.folderChangedCookie)
	, itemsChangedCookie(rhs.itemsChangedCookie)
{
	::ZeroMemory(&rhs, sizeof(rhs));
}

anim::FlatProjectItems::Entry::~Entry()
{
	if (this->folder != nullptr)
	{
		this->folder->PropertyChanged -= this->folderChangedCookie;
		this->folder->BindableFlatItems->VectorChanged -= this->itemsChangedCookie;
	}
}

anim::FlatProjectItems::Iterator::Iterator(Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^items)
	: items(items)
	, pos(0)
{
}

Platform::Object ^anim::FlatProjectItems::Iterator::Current::get()
{
	if (!this->HasCurrent)
	{
		return nullptr;
	}

	if (this->nested != nullptr)
	{
		return this->nested->Current;
	}

	return this->items->GetAt(this->pos);
}

bool anim::FlatProjectItems::Iterator::HasCurrent::get()
{
	return this->pos < this->items->Size;
}

bool anim::FlatProjectItems::Iterator::MoveNext()
{
	if (this->nested != nullptr)
	{
		if (!this->nested->MoveNext())
		{
			this->nested = nullptr;
			this->pos++;
		}
	}
	else
	{
		ProjectFolderVM ^folder = this->items->GetAt(this->pos)->AsFolder;
		if (folder != nullptr && folder->ShowExpanded && folder->BindableFlatItems->Size > 0)
		{
			this->nested = folder->BindableFlatItems->First();
		}
		else
		{
			this->pos++;
		}
	}

	return this->HasCurrent;
}
