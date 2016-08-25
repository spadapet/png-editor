#include "pch.h"
#include "View/Utility/FlatProjectItem.h"
#include "View/Utility/FlatProjectList.h"

anim::FlatProjectList::FlatProjectList()
{
}

anim::FlatProjectList::~FlatProjectList()
{
}

Windows::UI::Xaml::DependencyObject ^anim::FlatProjectList::GetContainerForItemOverride()
{
	return ref new FlatProjectItem(this);
}
