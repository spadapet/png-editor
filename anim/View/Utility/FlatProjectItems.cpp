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
{
	for (IProjectItemVM ^item : items)
	{
		ProjectFolderVM ^folder = item->AsFolder;
		if (folder != nullptr)
		{
			entries.emplace_back(this, folder);
		}
	}
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
	throw ref new Platform::NotImplementedException();
}

unsigned int anim::FlatProjectItems::Size::get()
{
	if (this->cachedSize == 0)
	{
		for (IProjectItemVM ^item : items)
		{
			this->cachedSize++;

			ProjectFolderVM ^folder = item->AsFolder;
			if (folder != nullptr && folder->ShowExpanded)
			{
				this->cachedSize += folder->BindableFlatItems->Size;
			}
		}
	}

	return this->cachedSize;
}

Platform::Object ^anim::FlatProjectItems::GetAt(unsigned int index)
{
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

void anim::FlatProjectItems::OnFolderChanged(Platform::String ^name)
{
	bool allChanged = (name == nullptr || name->Length() == 0);

	if (allChanged || name == "ShowExpanded")
	{
	}
}

void anim::FlatProjectItems::OnFolderItemsChanged(
	ProjectFolderVM ^folder,
	Windows::UI::Xaml::Interop::IBindableObservableVector ^items,
	Windows::Foundation::Collections::IVectorChangedEventArgs ^args)
{
}

anim::FlatProjectItems::Entry::Entry(FlatProjectItems ^owner, ProjectFolderVM ^folder)
	: folder(folder)
{
	Platform::WeakReference weakOwner(owner);

	this->folderChangedCookie = this->folder->PropertyChanged +=
		ref new Windows::UI::Xaml::Data::PropertyChangedEventHandler(
			[weakOwner](Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args)
	{
		FlatProjectItems ^owner = weakOwner.Resolve<FlatProjectItems>();
		if (owner != nullptr)
		{
			owner->OnFolderChanged(args->PropertyName);
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
