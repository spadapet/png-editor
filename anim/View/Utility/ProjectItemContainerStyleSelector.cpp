#include "pch.h"
#include "View/Utility/ProjectItemContainerStyleSelector.h"
#include "ViewModel/ProjectFileVM.h"
#include "ViewModel/ProjectFolderVM.h"
#include "ViewModel/ProjectItemVM.h"

anim::ProjectItemContainerStyleSelector::ProjectItemContainerStyleSelector()
{
}

Windows::UI::Xaml::Style ^anim::ProjectItemContainerStyleSelector::SelectStyleCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container)
{
	ProjectFolderVM ^folderVM = dynamic_cast<ProjectFolderVM ^>(item);
	ProjectFileVM ^fileVM = dynamic_cast<ProjectFileVM ^>(item);
	ProjectItemVM ^itemVM = dynamic_cast<ProjectItemVM ^>(item);

	if (folderVM != nullptr)
	{
		return (folderVM->Level == 0)
			? this->RootFolderStyle
			: this->FolderStyle;
	}

	if (fileVM != nullptr)
	{
		return this->FileStyle;
	}

	if (itemVM != nullptr)
	{
		return this->ItemStyle;
	}

	return nullptr;
}

Windows::UI::Xaml::Style ^anim::ProjectItemContainerStyleSelector::RootFolderStyle::get()
{
	return (this->rootFolderStyle != nullptr)
		? this->rootFolderStyle
		: this->FolderStyle;
}

void anim::ProjectItemContainerStyleSelector::RootFolderStyle::set(Windows::UI::Xaml::Style ^value)
{
	this->rootFolderStyle = value;
}

Windows::UI::Xaml::Style ^anim::ProjectItemContainerStyleSelector::FolderStyle::get()
{
	return (this->folderStyle != nullptr)
		? this->folderStyle
		: this->ItemStyle;
}

void anim::ProjectItemContainerStyleSelector::FolderStyle::set(Windows::UI::Xaml::Style ^value)
{
	this->folderStyle = value;
}

Windows::UI::Xaml::Style ^anim::ProjectItemContainerStyleSelector::FileStyle::get()
{
	return (this->fileStyle != nullptr)
		? this->fileStyle
		: this->ItemStyle;
}

void anim::ProjectItemContainerStyleSelector::FileStyle::set(Windows::UI::Xaml::Style ^value)
{
	this->fileStyle = value;
}

Windows::UI::Xaml::Style ^anim::ProjectItemContainerStyleSelector::ItemStyle::get()
{
	return this->itemStyle;
}

void anim::ProjectItemContainerStyleSelector::ItemStyle::set(Windows::UI::Xaml::Style ^value)
{
	this->itemStyle = value;
}
