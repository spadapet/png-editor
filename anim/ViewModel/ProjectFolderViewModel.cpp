#include "pch.h"
#include "ViewModel/ProjectFolderViewModel.h"

anim::ProjectFolderViewModel::ProjectFolderViewModel(Windows::Storage::StorageFolder ^folder)
	: folder(folder)
{
}

anim::ProjectFolderViewModel::~ProjectFolderViewModel()
{
}

void anim::ProjectFolderViewModel::Dispose()
{
}

Windows::Storage::StorageFolder ^anim::ProjectFolderViewModel::Folder::get()
{
	return this->folder;
}

Platform::String ^anim::ProjectFolderViewModel::DisplayName::get()
{
	return this->folder->Name;;
}

Platform::String ^anim::ProjectFolderViewModel::FullName::get()
{
	return this->folder->DisplayName;
}
