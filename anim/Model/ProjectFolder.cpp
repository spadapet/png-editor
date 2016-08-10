#include "pch.h"
#include "Model/ProjectFolder.h"

anim::ProjectFolder::ProjectFolder(Windows::Storage::StorageFolder ^folder)
	: folder(folder)
{
}

anim::ProjectFolder::~ProjectFolder()
{
}

Windows::Storage::StorageFolder ^anim::ProjectFolder::GetFolder() const
{
	return this->folder;
}

bool anim::ProjectFolder::IsRoot() const
{
	return false;
}
