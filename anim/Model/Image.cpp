#include "pch.h"
#include "Model/Image.h"

anim::Image::Image()
{
}

anim::Image::Image(const void *bytes, size_t byteSize)
{
	png_image image;
	::memset(&image, 0, sizeof(image));
	image.version = PNG_IMAGE_VERSION;

	if (::png_image_begin_read_from_memory(&image, bytes, byteSize))
	{
		image.format = PNG_FORMAT_RGBA;
		::png_image_free(&image);
	}

}

anim::Image::~Image()
{
}
