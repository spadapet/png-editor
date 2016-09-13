#include "pch.h"
#include "Model/OpenImageFile.h"
#include "Model/ProjectFile.h"
#include "ViewModel/OpenImageVM.h"

anim::OpenImageVM::OpenImageVM(std::shared_ptr<OpenImageFile> file)
	: file(file)
{
}

anim::OpenImageVM::OpenImageVM()
{
}

anim::OpenImageVM::~OpenImageVM()
{
}

void anim::OpenImageVM::Destroy()
{
}

std::shared_ptr<anim::OpenImageFile> anim::OpenImageVM::Model::get()
{
	return this->file;
}

Platform::String ^anim::OpenImageVM::Name::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->GetFile()->Name : "<null>";
}

Platform::String ^anim::OpenImageVM::Tooltip::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->GetFile()->Path : "<null>";
}

bool anim::OpenImageVM::IsDirty::get()
{
	return false;
}

Windows::UI::Xaml::UIElement ^anim::OpenImageVM::UserInterface::get()
{
	return nullptr;
}

Windows::UI::Xaml::Input::ICommand ^anim::OpenImageVM::CloseCommand::get()
{
	return nullptr;
}

void anim::OpenImageVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
