#include "pch.h"
#include "Model/ProjectFile.h"

anim::ProjectFile::ProjectFile(Windows::Storage::StorageFile ^file, std::shared_ptr<ProjectFolder> parent)
	: ProjectItem(file, parent)
	, file(file)
{
}

anim::ProjectFile::~ProjectFile()
{
}

Windows::Storage::StorageFile ^anim::ProjectFile::GetFile() const
{
	return this->file;
}
