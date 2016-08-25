#include "pch.h"
#include "View/Utility/FlatProjectItem.h"
#include "ViewModel/IProjectItemVM.h"

anim::FlatProjectItem::FlatProjectItem()
	: FlatProjectItem(nullptr)
{
}

anim::FlatProjectItem::FlatProjectItem(FlatProjectList ^list)
	: weakList(list)
{
}

anim::FlatProjectItem::~FlatProjectItem()
{
}

void anim::FlatProjectItem::OnKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args)
{
	Windows::UI::Xaml::Controls::ListBoxItem::OnKeyDown(args);
}

anim::FlatProjectList ^anim::FlatProjectItem::List::get()
{
	return this->weakList.Resolve<FlatProjectList>();
}

anim::IProjectItemVM ^anim::FlatProjectItem::Item::get()
{
	return dynamic_cast<IProjectItemVM ^>(this->DataContext);
}
