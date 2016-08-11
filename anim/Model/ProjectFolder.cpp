#include "pch.h"
#include "Model/ProjectFolder.h"

anim::ProjectFolder::ProjectFolder(Windows::Storage::StorageFolder ^folder, std::shared_ptr<ProjectFolder> parent)
	: ProjectItem(folder, parent)
	, folder(folder)
{
}

anim::ProjectFolder::~ProjectFolder()
{
}

Windows::Storage::StorageFolder ^anim::ProjectFolder::GetFolder() const
{
	return this->folder;
}

void anim::ProjectFolder::SetFolder(Windows::Storage::StorageFolder ^folder)
{
	if (folder != this->folder)
	{
		this->folder = folder;
		this->SetItem(folder);
		this->PropertyChanged.Notify("Folder");
	}
}
