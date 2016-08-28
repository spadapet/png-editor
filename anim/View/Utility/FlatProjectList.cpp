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
	this->SelectedItems->Clear();
	this->SelectedItems->Append(item);
	this->ScrollIntoView(item);
}

Windows::UI::Xaml::DependencyObject ^anim::FlatProjectList::GetContainerForItemOverride()
{
	return ref new FlatProjectItem(this);
}
