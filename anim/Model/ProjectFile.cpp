#include "pch.h"
#include "Model/ProjectFile.h"

anim::ProjectFile::ProjectFile(Windows::Storage::StorageFile ^file, std::shared_ptr<ProjectItem> parent)
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

void anim::ProjectFile::SetItem(Windows::Storage::IStorageItem ^item)
{
	Windows::Storage::StorageFile ^file = dynamic_cast<Windows::Storage::StorageFile ^>(item);
	assert(file != nullptr);

	if (file != nullptr)
	{
		this->SetFile(file);
	}
}

bool anim::ProjectFile::IsFile() const
{
	return true;
}

void anim::ProjectFile::SetFile(Windows::Storage::StorageFile ^file)
{
	if (file != this->file)
	{
		this->file = file;
		ProjectItem::SetItem(file);
		this->PropertyChanged.Notify("File");
	}
}
