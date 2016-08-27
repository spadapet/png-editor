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
	int index = this->IndexFromContainer(item);
	for (int i = index - 1; i >= 0; i--)
	{
		FlatProjectItem ^prevItem = dynamic_cast<FlatProjectItem ^>(this->ContainerFromIndex(i));
		if (prevItem != nullptr)
		{
			ProjectFolderVM ^folder = prevItem->Item->AsFolder;
			unsigned int index;
			if (folder != nullptr && folder->ShowExpanded && folder->Items->IndexOf(item->Item, &index))
			{
				return prevItem;
			}
		}
	}

	return nullptr;
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
