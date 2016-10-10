#include "pch.h"
#include "View/Utility/FlatProjectItem.h"
#include "View/Utility/FlatProjectList.h"
#include "ViewModel/IProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

anim::FlatProjectList::FlatProjectList()
{
}

anim::FlatProjectList::~FlatProjectList()
{
}

void anim::FlatProjectList::SelectSingle(IProjectItemVM ^item)
{
	Platform::Object ^obj = item;
	Platform::ArrayReference<Platform::Object ^> items(&obj, 1);

	this->SelectedItems->ReplaceAll(items);
	this->ScrollIntoView(item);
}

Windows::UI::Xaml::DependencyObject ^anim::FlatProjectList::GetContainerForItemOverride()
{
	return ref new FlatProjectItem(this);
}
