#include "pch.h"
#include "Core/Xaml.h"
#include "Model/ProjectItem.h"
#include "ViewModel/ProjectItemVM.h"

anim::ProjectItemVM::ProjectItemVM(std::shared_ptr<ProjectItem> item)
	: item(item)
{
}

anim::ProjectItemVM::ProjectItemVM()
	: ProjectItemVM(std::shared_ptr<ProjectItem>())
{
	anim::AssertXamlDesigner();
}

anim::ProjectItemVM::~ProjectItemVM()
{
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
	return (this->item != nullptr) ? this->item->GetLevel() : 0;
}

anim::ProjectFileVM ^anim::ProjectItemVM::AsFile::get()
{
	return nullptr;
}

anim::ProjectFolderVM ^anim::ProjectItemVM::AsFolder::get()
{
	return nullptr;
}

bool anim::ProjectItemVM::OnActivate()
{
	return false;
}

bool anim::ProjectItemVM::OnExpand()
{
	return false;
}

bool anim::ProjectItemVM::OnCollapse()
{
	return false;
}

void anim::ProjectItemVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
