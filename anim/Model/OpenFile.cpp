#include "pch.h"
#include "Model/OpenFile.h"
#include "Model/ProjectFile.h"

anim::OpenFile::OpenFile(std::shared_ptr<ProjectFile> file)
	: file(file)
{
}

anim::OpenFile::~OpenFile()
{
}

void anim::OpenFile::Initialize()
{
	this->file->SetOpenFile(this->shared_from_this());
}

void anim::OpenFile::Destroy()
{
	this->file->SetOpenFile(nullptr);
}
