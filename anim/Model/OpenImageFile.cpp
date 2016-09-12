#include "pch.h"
#include "Model/OpenImageFile.h"

anim::OpenImageFile::OpenImageFile(std::shared_ptr<ProjectFile> file)
	: OpenFile(file)
{
}

anim::OpenImageFile::~OpenImageFile()
{
}

void anim::OpenImageFile::Initialize()
{
	OpenFile::Initialize();
}

void anim::OpenImageFile::Destroy()
{
	OpenFile::Destroy();
}
