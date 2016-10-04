#include "pch.h"
#include "Core/String.h"
#include "Model/Image.h"

anim::Image::Image()
	: pngStruct(nullptr)
	, pngInfo(nullptr)
	, pngEndInfo(nullptr)
	, readBytes(nullptr)
	, endReadBytes(nullptr)
{
}

anim::Image::~Image()
{
	if (this->pngStruct != nullptr)
	{
		::png_destroy_read_struct(&this->pngStruct, &this->pngInfo, &this->pngEndInfo);
	}
}

bool anim::Image::Initialize(const void *bytes, size_t byteSize, std::string &errorText)
{
	this->readBytes = (const unsigned char *)bytes;
	this->endReadBytes = this->readBytes + byteSize;

	if (::png_sig_cmp((const unsigned char *)bytes, 0, byteSize) != 0)
	{
		errorText = anim::ConvertString(anim::Resource::GetString("ErrorInvalidPng"));
		return false;
	}

	this->pngStruct = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, this, &Image::PngErrorCallback, &Image::PngWarningCallback);
	this->pngInfo = ::png_create_info_struct(this->pngStruct);
	this->pngEndInfo = ::png_create_info_struct(this->pngStruct);

	::png_set_chunk_malloc_max(this->pngStruct, 0x10000000);
	::png_set_read_user_chunk_fn(this->pngStruct, this, &Image::PngUnknownChunkCallback);
	::png_set_read_fn(this->pngStruct, this, &Image::PngReadCallback);

	if (::setjmp(png_jmpbuf(this->pngStruct)))
	{
		errorText = anim::ConvertString(anim::Resource::GetString("ErrorFailedPng"));
		return false;
	}

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

void anim::Image::PngReadCallback(png_struct *pngStruct, unsigned char *data, size_t size)
{
	Image *owner = (Image *)::png_get_io_ptr(pngStruct);
	owner->OnPngRead(pngStruct, data, size);
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

void anim::Image::OnPngRead(png_struct *pngStruct, unsigned char *data, size_t size)
{
}
