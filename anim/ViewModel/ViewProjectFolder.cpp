#include "pch.h"
#include "ViewModel/ViewProjectFolder.h"

anim::ViewProjectFolder::ViewProjectFolder(Windows::Storage::StorageFolder ^folder)
	: folder(folder)
{
}

anim::ViewProjectFolder::~ViewProjectFolder()
{
}

void anim::ViewProjectFolder::Dispose()
{
}

Windows::Storage::StorageFolder ^anim::ViewProjectFolder::Folder::get()
{
	return this->folder;
}

Platform::String ^anim::ViewProjectFolder::DisplayName::get()
{
	return this->folder->Name;;
}

Platform::String ^anim::ViewProjectFolder::FullName::get()
{
	return this->folder->DisplayName;
}
