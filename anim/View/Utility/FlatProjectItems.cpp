#include "pch.h"
#include "Model/AppState.h"
#include "View/Utility/FlatProjectItems.h"
#include "ViewModel/IProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

anim::FlatProjectItems::FlatProjectItems()
	: FlatProjectItems(ref new Platform::Collections::Vector<IProjectItemVM ^>())
{
}

anim::FlatProjectItems::FlatProjectItems(Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^roots)
{
}

anim::FlatProjectItems::~FlatProjectItems()
{
}

Windows::UI::Xaml::Interop::IBindableIterator ^anim::FlatProjectItems::First()
{
	throw ref new Platform::NotImplementedException();
}

unsigned int anim::FlatProjectItems::Size::get()
{
	return 0;
}

Platform::Object ^anim::FlatProjectItems::GetAt(unsigned int index)
{
	throw ref new Platform::NotImplementedException();
}

Windows::UI::Xaml::Interop::IBindableVectorView ^anim::FlatProjectItems::GetView()
{
	throw ref new Platform::NotImplementedException();
}

bool anim::FlatProjectItems::IndexOf(Platform::Object ^value, unsigned int *index)
{
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

anim::FlatProjectItems::Entry::Entry(Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^items)
{
}

anim::FlatProjectItems::Entry::~Entry()
{
}
