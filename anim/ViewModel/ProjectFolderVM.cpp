#include "pch.h"
#include "Core/Designer.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFolderVM.h"

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder)
	: folder(folder)
{
}

anim::ProjectFolderVM::ProjectFolderVM()
	: ProjectFolderVM(nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
}

Windows::Storage::StorageFolder ^anim::ProjectFolderVM::Folder::get()
{
	return this->folder->GetFolder();
}

Platform::String ^anim::ProjectFolderVM::DisplayName::get()
{
	return this->folder->GetFolder()->Name;
}

Platform::String ^anim::ProjectFolderVM::FullName::get()
{
	return this->folder->GetFolder()->DisplayName;
}

void anim::ProjectFolderVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
