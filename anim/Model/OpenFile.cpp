#include "pch.h"
#include "Model/OpenFile.h"

anim::OpenFile::OpenFile(std::shared_ptr<ProjectFile> file)
	: file(file)
{
}

anim::OpenFile::~OpenFile()
{
}
