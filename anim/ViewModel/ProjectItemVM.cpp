#include "pch.h"
#include "Core/Xaml.h"
#include "Model/ProjectItem.h"
#include "ViewModel/ProjectItemVM.h"

anim::ProjectItemVM::ProjectItemVM(std::shared_ptr<ProjectItem> item, ProjectFolderVM ^parent)
	: item(item)
	, weakParent(parent != nullptr ? Platform::WeakReference(parent) : Platform::WeakReference(nullptr))
{
}

anim::ProjectItemVM::ProjectItemVM()
	: ProjectItemVM(std::shared_ptr<ProjectItem>(), nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectItemVM::~ProjectItemVM()
{
}

void anim::ProjectItemVM::Destroy()
{
	this->item = nullptr;
	this->weakParent = nullptr;
	this->NotifyPropertyChanged();
}

Windows::Storage::IStorageItem ^anim::ProjectItemVM::Item::get()
{
	return (this->item != nullptr) ? this->item->GetItem() : nullptr;
}

Platform::String ^anim::ProjectItemVM::DisplayName::get()
{
	return (this->item != nullptr) ? this->item->GetItem()->Name : "<null>";
}

Platform::String ^anim::ProjectItemVM::FullPath::get()
{
	return (this->item != nullptr) ? this->item->GetItem()->Path : "<null>";
}

int anim::ProjectItemVM::Level::get()
{
	IProjectItemVM ^parent = this->Parent;
	return (parent != nullptr) ? parent->Level + 1 : 0;
}

anim::ProjectFileVM ^anim::ProjectItemVM::AsFile::get()
{
	return nullptr;
}

anim::ProjectFolderVM ^anim::ProjectItemVM::AsFolder::get()
{
	return nullptr;
}

anim::IProjectItemVM ^anim::ProjectItemVM::Parent::get()
{
	return this->weakParent.Resolve<IProjectItemVM>();
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectItemVM::ActivateCommand::get()
{
	return nullptr;
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectItemVM::DeleteCommand::get()
{
	return nullptr;
}

void anim::ProjectItemVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
