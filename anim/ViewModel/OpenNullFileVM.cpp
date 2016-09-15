#include "pch.h"
#include "ViewModel/OpenNullFileVM.h"

anim::OpenNullFileVM::OpenNullFileVM()
	: active(false)
{
}

anim::OpenNullFileVM::~OpenNullFileVM()
{
}

void anim::OpenNullFileVM::Destroy()
{
}

Platform::String ^anim::OpenNullFileVM::Name::get()
{
	return "";
}

Platform::String ^anim::OpenNullFileVM::Tooltip::get()
{
	return "";
}

Platform::String ^anim::OpenNullFileVM::Path::get()
{
	return "";
}

bool anim::OpenNullFileVM::IsDirty::get()
{
	return false;
}

bool anim::OpenNullFileVM::IsActive::get()
{
	return this->active;
}

void anim::OpenNullFileVM::IsActive::set(bool value)
{
	if (this->active != value)
	{
		this->active = value;
		this->NotifyPropertyChanged("IsActive");
	}
}

bool anim::OpenNullFileVM::TabMouseHover::get()
{
	return false;
}

void anim::OpenNullFileVM::TabMouseHover::set(bool value)
{
	assert(false);
}

bool anim::OpenNullFileVM::ShowCloseButton::get()
{
	return false;
}

Windows::UI::Xaml::UIElement ^anim::OpenNullFileVM::UserInterface::get()
{
	return nullptr;
}

Windows::UI::Xaml::Input::ICommand ^anim::OpenNullFileVM::CloseCommand::get()
{
	return nullptr;
}

anim::OpenImageVM ^anim::OpenNullFileVM::AsImage::get()
{
	return nullptr;
}

void anim::OpenNullFileVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
