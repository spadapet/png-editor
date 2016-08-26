#include "pch.h"
#include "Core/Xaml.h"
#include "Model/ProjectFile.h"
#include "ViewModel/ProjectFileVM.h"

anim::ProjectFileVM::ProjectFileVM(std::shared_ptr<ProjectFile> file)
	: file(file)
{
}

anim::ProjectFileVM::ProjectFileVM()
	: ProjectFileVM(std::shared_ptr<ProjectFile>())
{
	anim::AssertXamlDesigner();
}

anim::ProjectFileVM::~ProjectFileVM()
{
}

Windows::Storage::IStorageItem ^anim::ProjectFileVM::Item::get()
{
	return (this->file != nullptr) ? this->file->GetItem() : nullptr;
}

Platform::String ^anim::ProjectFileVM::DisplayName::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->Name : "<null>";
}

Platform::String ^anim::ProjectFileVM::FullPath::get()
{
	return (this->file != nullptr) ? this->file->GetItem()->Path : "<null>";
}

int anim::ProjectFileVM::Level::get()
{
	return (this->file != nullptr) ? this->file->GetLevel() : 0;
}

anim::ProjectFileVM ^anim::ProjectFileVM::AsFile::get()
{
	return this;
}

anim::ProjectFolderVM ^anim::ProjectFileVM::AsFolder::get()
{
	return nullptr;
}

bool anim::ProjectFileVM::OnActivate()
{
	return false;
}

bool anim::ProjectFileVM::OnExpand()
{
	return false;
}

bool anim::ProjectFileVM::OnCollapse()
{
	return false;
}

Windows::Storage::StorageFile ^anim::ProjectFileVM::File::get()
{
	return (this->file != nullptr) ? this->file->GetFile() : nullptr;
}

bool anim::ProjectFileVM::IsOpen::get()
{
	return false;
}

void anim::ProjectFileVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
