#include "pch.h"
#include "View/Utility/OpenFileUiTemplateSelector.h"
#include "ViewModel/OpenImageVM.h"
#include "ViewModel/OpenNullFileVM.h"

anim::OpenFileUiTemplateSelector::OpenFileUiTemplateSelector()
{
}

Windows::UI::Xaml::DataTemplate ^anim::OpenFileUiTemplateSelector::SelectTemplateCore(Platform::Object ^item)
{
	OpenImageVM ^imageVM = dynamic_cast<OpenImageVM ^>(item);
	OpenNullFileVM ^nullVM = dynamic_cast<OpenNullFileVM ^>(item);

	if (imageVM != nullptr)
	{
		return this->ImageTemplate;
	}

	if (nullVM != nullptr)
	{
		return this->NullTemplate;
	}

	if (item != nullptr)
	{
		return this->UnknownTemplate;
	}

	return nullptr;
}

Windows::UI::Xaml::DataTemplate ^anim::OpenFileUiTemplateSelector::SelectTemplateCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container)
{
	return this->SelectTemplateCore(item);
}

Windows::UI::Xaml::DataTemplate ^anim::OpenFileUiTemplateSelector::ImageTemplate::get()
{
	return this->imageTemplate != nullptr
		? this->imageTemplate
		: this->UnknownTemplate;
}

void anim::OpenFileUiTemplateSelector::ImageTemplate::set(Windows::UI::Xaml::DataTemplate ^value)
{
	this->imageTemplate = value;
}

Windows::UI::Xaml::DataTemplate ^anim::OpenFileUiTemplateSelector::UnknownTemplate::get()
{
	return this->unknownTemplate;
}

void anim::OpenFileUiTemplateSelector::UnknownTemplate::set(Windows::UI::Xaml::DataTemplate ^value)
{
	this->unknownTemplate = value;
}

Windows::UI::Xaml::DataTemplate ^anim::OpenFileUiTemplateSelector::NullTemplate::get()
{
	return this->nullTemplate != nullptr
		? this->nullTemplate
		: this->UnknownTemplate;
}

void anim::OpenFileUiTemplateSelector::NullTemplate::set(Windows::UI::Xaml::DataTemplate ^value)
{
	this->nullTemplate = value;
}
