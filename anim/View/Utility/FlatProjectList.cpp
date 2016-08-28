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

anim::FlatProjectItem ^anim::FlatProjectList::GetParent(FlatProjectItem ^item)
{
	IProjectItemVM ^parent = item->Item->Parent;
	return dynamic_cast<FlatProjectItem ^>(this->ContainerFromItem(parent));
}

anim::FlatProjectItem ^anim::FlatProjectList::GetChild(FlatProjectItem ^item)
{
	ProjectFolderVM ^folder = item->Item->AsFolder;
	if (folder != nullptr && folder->ShowExpanded && folder->Items->Size > 0)
	{
		IProjectItemVM ^child = folder->Items->GetAt(0);
		return dynamic_cast<FlatProjectItem ^>(this->ContainerFromItem(child));
	}

	return nullptr;
}

Windows::UI::Xaml::DependencyObject ^anim::FlatProjectList::GetContainerForItemOverride()
{
	return ref new FlatProjectItem(this);
}
