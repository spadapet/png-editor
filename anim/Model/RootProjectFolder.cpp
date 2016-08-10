#include "pch.h"
#include "Model/RootProjectFolder.h"

anim::RootProjectFolder::RootProjectFolder(Windows::Storage::StorageFolder ^folder)
	: ProjectFolder(folder)
{
}

anim::RootProjectFolder::~RootProjectFolder()
{
}

bool anim::RootProjectFolder::IsRoot() const
{
	return true;
}
