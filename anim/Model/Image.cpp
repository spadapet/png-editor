#include "pch.h"
#include "Core/String.h"
#include "Model/Image.h"

namespace anim
{
	struct ReadInfo
	{
		ReadInfo(anim::Image *image, const unsigned char *bytes, size_t size, std::string &errorText);
		~ReadInfo();

		bool Read();

		// Data
		Image *image;
		png_struct *png;
		png_info *info;
		png_info *endInfo;
		std::string &errorText;

		// Reading
		const unsigned char *readPos;
		const unsigned char *endPos;
		unsigned char *rowBuf;

		// Properties
		unsigned int width;
		unsigned int height;
		int bitDepth;
		int colorType;
		int interlaceMethod;
		png_color *palette;
		int paletteSize;
		unsigned char *transPalette;
		int transPaletteSize;
		png_color_16 *transColor;
	};
}

anim::ReadInfo::ReadInfo(Image *image, const unsigned char *bytes, size_t size, std::string &errorText)
	: image(image)
	, readPos(bytes)
	, endPos(bytes + size)
	, rowBuf(nullptr)
	, errorText(errorText)
	, width(0)
	, height(0)
	, bitDepth(0)
	, colorType(0)
	, interlaceMethod(0)
	, palette(nullptr)
	, paletteSize(0)
	, transPalette(nullptr)
	, transPaletteSize(0)
	, transColor(nullptr)
{
	this->png = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, this, &Image::PngErrorCallback, &Image::PngWarningCallback);
	this->info = ::png_create_info_struct(this->png);
	this->endInfo = ::png_create_info_struct(this->png);
}

anim::ReadInfo::~ReadInfo()
{
	if (this->rowBuf != nullptr)
	{
		::png_free(this->png, this->rowBuf);
	}

	::png_destroy_read_struct(&this->png, &this->info, &this->endInfo);
}

bool anim::ReadInfo::Read()
{
	if (::png_sig_cmp(this->readPos, 0, this->endPos - this->readPos) != 0)
	{
		this->errorText = anim::ConvertString(anim::Resource::GetString("ErrorInvalidPng"));
		return false;
	}

	::png_set_chunk_malloc_max(this->png, 0x10000000);
	::png_set_read_fn(this->png, this, &Image::PngReadCallback);
	::png_set_read_user_chunk_fn(this->png, this, &Image::PngUnknownChunkCallback);
	::png_set_read_status_fn(this->png, &Image::PngReadStatusCallback);
	::png_set_keep_unknown_chunks(this->png, PNG_HANDLE_CHUNK_ALWAYS, nullptr, 0);

	::png_read_info(this->png, this->info);

	// General image info
	if (!::png_get_IHDR(
		this->png,
		this->info,
		&this->width,
		&this->height,
		&this->bitDepth,
		&this->colorType,
		&this->interlaceMethod,
		nullptr,
		nullptr))
	{
		return false;
	}

	// Palette
	::png_get_PLTE(this->png, this->info, &this->palette, &this->paletteSize);
	::png_get_tRNS(this->png, this->info, &this->transPalette, &this->transPaletteSize, &this->transColor);

	this->rowBuf = (unsigned char *)::png_malloc(this->png, ::png_get_rowbytes(this->png, this->info));

	return true;
}

anim::Image::Image()
{
}

anim::Image::~Image()
{
}

bool anim::Image::Initialize(const void *bytes, size_t byteSize, std::string &errorText)
{
	ReadInfo info(this, (const unsigned char *)bytes, byteSize, errorText);

	if (!info.Read())
	{
		return false;
	}

	return true;
}

void anim::Image::PngErrorCallback(png_struct *png, const char *text)
{
	ReadInfo *info = (ReadInfo *)::png_get_error_ptr(png);
	info->image->OnPngError(*info, text);
}

void anim::Image::PngWarningCallback(png_struct *png, const char *text)
{
	ReadInfo *info = (ReadInfo *)::png_get_error_ptr(png);
	info->image->OnPngWarning(*info, text);
}

int anim::Image::PngUnknownChunkCallback(png_struct *png, png_unknown_chunk *chunk)
{
	ReadInfo *info = (ReadInfo *)::png_get_user_chunk_ptr(png);
	return info->image->OnPngUnknownChunk(*info, chunk);
}

void anim::Image::PngReadCallback(png_struct *png, unsigned char *data, size_t size)
{
	ReadInfo *info = (ReadInfo *)::png_get_io_ptr(png);
	info->image->OnPngRead(*info, data, size);
}

void anim::Image::PngReadStatusCallback(png_struct *png, unsigned int row, int pass)
{
	ReadInfo *info = (ReadInfo *)::png_get_io_ptr(png);
	info->image->OnPngReadStatus(*info, row, pass);
}

void anim::Image::OnPngError(ReadInfo &info, const char *text)
{
	throw ref new Platform::Exception(E_FAIL, anim::Resource::GetString("ErrorFailedPng"));
}

void anim::Image::OnPngWarning(ReadInfo &info, const char *text)
{
}

int anim::Image::OnPngUnknownChunk(ReadInfo &info, png_unknown_chunk *chunk)
{
	return 0;
}

void anim::Image::OnPngRead(ReadInfo &info, unsigned char *data, size_t size)
{
	if (size > (size_t)(info.endPos - info.readPos))
	{
		size = info.endPos - info.readPos;
	}

	::memcpy(data, info.readPos, size);
	info.readPos += size;
}

void anim::Image::OnPngReadStatus(ReadInfo &info, unsigned int row, int pass)
{
}
