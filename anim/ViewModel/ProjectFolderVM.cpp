#include "pch.h"
#include "ViewModel/ProjectFolderVM.h"

anim::ProjectFolderVM::ProjectFolderVM(Windows::Storage::StorageFolder ^folder)
	: folder(folder)
{
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
}

Windows::Storage::StorageFolder ^anim::ProjectFolderVM::Folder::get()
{
	return this->folder;
}

Platform::String ^anim::ProjectFolderVM::DisplayName::get()
{
	return this->folder->Name;;
}

Platform::String ^anim::ProjectFolderVM::FullName::get()
{
	return this->folder->DisplayName;
}
