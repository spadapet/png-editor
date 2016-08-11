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

void anim::ProjectFile::SetFile(Windows::Storage::StorageFile ^file)
{
	if (file != this->file)
	{
		this->file = file;
		this->SetItem(file);
		this->PropertyChanged.Notify("File");
	}
}
