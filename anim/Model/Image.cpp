#include "pch.h"
#include "Model/Image.h"

anim::Image::Image()
	: pngStruct(nullptr)
	, pngInfo(nullptr)
	, pngEndInfo(nullptr)
{
}

anim::Image::~Image()
{
	if (this->pngStruct != nullptr)
	{
		::png_destroy_read_struct(&this->pngStruct, &this->pngInfo, &this->pngEndInfo);
	}
}

bool anim::Image::Initialize(const void * bytes, size_t byteSize, std::string &errorText)
{
	if (::png_sig_cmp((const unsigned char *)bytes, 0, byteSize) != 0)
	{
		assert(false);
		return false;
	}

	this->pngStruct = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, this, &Image::PngErrorCallback, &Image::PngWarningCallback);
	if (this->pngStruct == nullptr)
	{
		assert(false);
		return false;
	}

	::png_set_read_user_chunk_fn(this->pngStruct, this, &Image::PngUnknownChunkCallback);

	return true;
}

void anim::Image::PngErrorCallback(png_struct *pngStruct, const char *text)
{
	Image *owner = (Image *)::png_get_error_ptr(pngStruct);
	owner->OnPngError(pngStruct, text);
}

void anim::Image::PngWarningCallback(png_struct *pngStruct, const char *text)
{
	Image *owner = (Image *)::png_get_error_ptr(pngStruct);
	owner->OnPngWarning(pngStruct, text);
}

int anim::Image::PngUnknownChunkCallback(png_struct *pngStruct, png_unknown_chunk *chunk)
{
	Image *owner = (Image *)::png_get_error_ptr(pngStruct);
	return owner->OnPngUnknownChunk(pngStruct, chunk);
}

void anim::Image::OnPngError(png_struct *pngStruct, const char *text)
{
}

void anim::Image::OnPngWarning(png_struct *pngStruct, const char *text)
{
}

int anim::Image::OnPngUnknownChunk(png_struct *pngStruct, png_unknown_chunk *chunk)
{
	return 0;
}
