#include "pch.h"
#include "Core/Designer.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFolderVM.h"

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder)
	: folder(folder)
	, folders(ref new Platform::Collections::Vector<ProjectFolderVM ^>())
	, files(ref new Platform::Collections::Vector<ProjectFileVM ^>())
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
	return this->folder->GetFolder()->DisplayName;
}

Platform::String ^anim::ProjectFolderVM::FullPath::get()
{
	return this->folder->GetFolder()->Path;
}

Windows::Foundation::Collections::IVector<anim::ProjectFolderVM ^> ^anim::ProjectFolderVM::Folders::get()
{
	return this->folders;
}

Windows::Foundation::Collections::IVector<anim::ProjectFileVM ^> ^anim::ProjectFolderVM::Files::get()
{
	return this->files;
}

void anim::ProjectFolderVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
