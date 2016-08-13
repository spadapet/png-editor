#include "pch.h"
#include "View/Utility/ProjectItemTemplateSelector.h"
#include "ViewModel/ProjectFileVM.h"
#include "ViewModel/ProjectFolderVM.h"
#include "ViewModel/ProjectItemVM.h"

anim::ProjectItemTemplateSelector::ProjectItemTemplateSelector()
{
}

Windows::UI::Xaml::DataTemplate ^anim::ProjectItemTemplateSelector::SelectTemplateCore(Platform::Object ^item)
{
	ProjectFolderVM ^folderVM = dynamic_cast<ProjectFolderVM ^>(item);
	ProjectFileVM ^fileVM = dynamic_cast<ProjectFileVM ^>(item);
	ProjectItemVM ^itemVM = dynamic_cast<ProjectItemVM ^>(item);

	if (folderVM != nullptr)
	{
		return (folderVM->Level == 0)
			? this->RootFolderTemplate
			: this->FolderTemplate;
	}

	if (fileVM != nullptr)
	{
		return this->FileTemplate;
	}

	if (itemVM != nullptr)
	{
		return this->ItemTemplate;
	}

	return nullptr;
}

Windows::UI::Xaml::DataTemplate ^anim::ProjectItemTemplateSelector::SelectTemplateCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container)
{
	return this->SelectTemplateCore(item);
}

Windows::UI::Xaml::DataTemplate ^anim::ProjectItemTemplateSelector::RootFolderTemplate::get()
{
	return (this->rootFolderTemplate != nullptr)
		? this->rootFolderTemplate
		: this->FolderTemplate;
}

void anim::ProjectItemTemplateSelector::RootFolderTemplate::set(Windows::UI::Xaml::DataTemplate ^value)
{
	this->rootFolderTemplate = value;
}

Windows::UI::Xaml::DataTemplate ^anim::ProjectItemTemplateSelector::FolderTemplate::get()
{
	return (this->folderTemplate != nullptr)
		? this->folderTemplate
		: this->ItemTemplate;
}

void anim::ProjectItemTemplateSelector::FolderTemplate::set(Windows::UI::Xaml::DataTemplate ^value)
{
	this->folderTemplate = value;
}

Windows::UI::Xaml::DataTemplate ^anim::ProjectItemTemplateSelector::FileTemplate::get()
{
	return (this->fileTemplate != nullptr)
		? this->fileTemplate
		: this->ItemTemplate;
}

void anim::ProjectItemTemplateSelector::FileTemplate::set(Windows::UI::Xaml::DataTemplate ^value)
{
	this->fileTemplate = value;
}

Windows::UI::Xaml::DataTemplate ^anim::ProjectItemTemplateSelector::ItemTemplate::get()
{
	return this->itemTemplate;
}

void anim::ProjectItemTemplateSelector::ItemTemplate::set(Windows::UI::Xaml::DataTemplate ^value)
{
	this->itemTemplate = value;
}
