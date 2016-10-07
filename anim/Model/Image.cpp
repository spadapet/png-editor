#include "pch.h"
#include "Core/PngReadInfo.h"
#include "Core/String.h"
#include "Model/Image.h"

anim::Image::Image()
{
}

anim::Image::~Image()
{
}

bool anim::Image::Initialize(const void *bytes, size_t byteSize, std::string &errorText)
{
	PngReadInfo info((const unsigned char *)bytes, byteSize);

	if (!info.Read())
	{
		errorText = info.GetErrorText();
		return false;
	}

	return true;
}
