#include "pch.h"
#include "Model/ProjectFile.h"

anim::ProjectFile::ProjectFile(Windows::Storage::StorageFile ^file)
	: file(file)
{
}

anim::ProjectFile::~ProjectFile()
{
}

Windows::Storage::StorageFile ^anim::ProjectFile::GetFile() const
{
	return this->file;
}
