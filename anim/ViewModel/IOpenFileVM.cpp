#include "pch.h"
#include "Model/OpenImageFile.h"
#include "ViewModel/IOpenFileVM.h"
#include "ViewModel/OpenImageVM.h"

std::shared_ptr<anim::OpenFile> anim::GetOpenFileModel(IOpenFileVM ^ file)
{
	std::shared_ptr<OpenFile> model;

	if (file != nullptr)
	{
		if (file->AsImage)
		{
			model = file->AsImage->Model;
		}
	}

	assert(model != nullptr);
	return model;
}
