#include "pch.h"
#include "Model/AppState.h"
#include "View/Utility/FlatProjectItems.h"
#include "ViewModel/IProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

anim::FlatProjectItems::FlatProjectItems()
	: FlatProjectItems(ref new Platform::Collections::Vector<IProjectItemVM ^>())
{
}

anim::FlatProjectItems::FlatProjectItems(Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^items)
	: items(items)
	, cachedSize(0)
	, cachedItemPos(0)
{
	// TODO: Add a root listener and update entries

	for (IProjectItemVM ^item : items)
	{
		ProjectFolderVM ^folder = item->AsFolder;
		if (folder != nullptr)
		{
			this->entries.emplace_back(this, folder);
		}
	}

	this->UpdateCachedSize();
}

anim::FlatProjectItems::~FlatProjectItems()
{
}

Windows::UI::Xaml::Interop::IBindableObservableVector ^anim::FlatProjectItems::BindableItems::get()
{
	return this;
}

Windows::UI::Xaml::Interop::IBindableIterator ^anim::FlatProjectItems::First()
{
	return ref new FlatProjectItems::Iterator(this->items);
}

unsigned int anim::FlatProjectItems::Size::get()
{
	return this->cachedSize;
}

Platform::Object ^anim::FlatProjectItems::GetAt(unsigned int index)
{
	if (this->cachedItem != nullptr && this->cachedItemPos == index)
	{
		return this->cachedItem;
	}

	unsigned int cur = 0;
	for (IProjectItemVM ^item : this->items)
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

	assert(false);
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
	for (IProjectItemVM ^item : this->items)
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

void anim::FlatProjectItems::OnFolderChanged(ProjectFolderVM ^folder, Platform::String ^name)
{
	bool allChanged = (name == nullptr || name->Length() == 0);

	if (allChanged || name == "ShowExpanded")
	{
		unsigned int flatIndex = this->FlatIndexOfChild(folder);
		unsigned int size = folder->BindableFlatItems->Size;

		if (folder->ShowExpanded)
		{
			this->cachedSize += size;

			for (unsigned int cur = flatIndex + 1; cur < flatIndex + 1 + size; cur++)
			{
				this->VectorChanged(this, ref new Platform::Collections::Details::VectorChangedEventArgs(
					Windows::Foundation::Collections::CollectionChange::ItemInserted, cur));
			}
		}
		else
		{
			this->cachedSize -= size;

			for (size_t i = 0; i < size; i++)
			{
				this->VectorChanged(this, ref new Platform::Collections::Details::VectorChangedEventArgs(
					Windows::Foundation::Collections::CollectionChange::ItemRemoved, flatIndex + 1));
			}
		}
	}
}

void anim::FlatProjectItems::OnFolderItemsChanged(
	ProjectFolderVM ^folder,
	Windows::UI::Xaml::Interop::IBindableObservableVector ^items,
	Windows::Foundation::Collections::IVectorChangedEventArgs ^args)
{
	if (!folder->ShowExpanded)
	{
		return;
	}

	unsigned int flatIndex = this->FlatIndexOfChild(folder);

	switch (args->CollectionChange)
	{
	case Windows::Foundation::Collections::CollectionChange::ItemChanged:
		this->VectorChanged(this, ref new Platform::Collections::Details::VectorChangedEventArgs(
			args->CollectionChange, flatIndex + 1 + args->Index));
		break;

	case Windows::Foundation::Collections::CollectionChange::ItemInserted:
		this->cachedSize++;
		this->VectorChanged(this, ref new Platform::Collections::Details::VectorChangedEventArgs(
			args->CollectionChange, flatIndex + 1 + args->Index));
		break;

	case Windows::Foundation::Collections::CollectionChange::ItemRemoved:
		this->cachedSize--;
		this->VectorChanged(this, ref new Platform::Collections::Details::VectorChangedEventArgs(
			args->CollectionChange, flatIndex + 1 + args->Index));
		break;

	case Windows::Foundation::Collections::CollectionChange::Reset:
		this->UpdateCachedSize();
		this->VectorChanged(this, ref new Platform::Collections::Details::VectorChangedEventArgs(
			args->CollectionChange, 0));
		break;
	}
}

void anim::FlatProjectItems::UpdateCachedSize()
{
	this->cachedSize = 0;

	for (IProjectItemVM ^item : items)
	{
		this->cachedSize++;

		ProjectFolderVM ^folder = item->AsFolder;
		if (folder != nullptr)
		{
			if (folder->ShowExpanded)
			{
				this->cachedSize += folder->BindableFlatItems->Size;
			}
		}
	}
}

void anim::FlatProjectItems::InvalidateItemCache()
{
	this->cachedItem = nullptr;
}

unsigned int anim::FlatProjectItems::FlatIndexOfChild(IProjectItemVM ^item)
{
	unsigned int flatIndex = 0;
	for (unsigned int i = 0; i < this->items->Size; i++, flatIndex++)
	{
		IProjectItemVM ^otherItem = this->items->GetAt(i);
		if (otherItem == item)
		{
			break;
		}

		ProjectFolderVM ^otherFolder = item->AsFolder;
		if (otherFolder->ShowExpanded)
		{
			flatIndex += otherFolder->BindableFlatItems->Size;
		}
	}

	return flatIndex;
}

anim::FlatProjectItems::Entry::Entry(FlatProjectItems ^owner, ProjectFolderVM ^folder)
	: folder(folder)
{
	Platform::WeakReference weakOwner(owner);

	this->folderChangedCookie = this->folder->PropertyChanged +=
		ref new Windows::UI::Xaml::Data::PropertyChangedEventHandler(
			[weakOwner, folder](Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args)
	{
		FlatProjectItems ^owner = weakOwner.Resolve<FlatProjectItems>();
		if (owner != nullptr)
		{
			owner->OnFolderChanged(folder, args->PropertyName);
		}
	});

	this->itemsChangedCookie = this->folder->BindableFlatItems->VectorChanged +=
		ref new Windows::UI::Xaml::Interop::BindableVectorChangedEventHandler(
			[weakOwner, folder](Windows::UI::Xaml::Interop::IBindableObservableVector ^items, Platform::Object ^argsObject)
	{
		FlatProjectItems ^owner = weakOwner.Resolve<FlatProjectItems>();
		Windows::Foundation::Collections::IVectorChangedEventArgs ^args = dynamic_cast<Windows::Foundation::Collections::IVectorChangedEventArgs ^>(argsObject);

		if (owner != nullptr && args != nullptr)
		{
			owner->OnFolderItemsChanged(folder, items, args);
		}
	});
}

anim::FlatProjectItems::Entry::~Entry()
{
	this->folder->PropertyChanged -= this->folderChangedCookie;
	this->folder->BindableFlatItems->VectorChanged -= this->itemsChangedCookie;
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
