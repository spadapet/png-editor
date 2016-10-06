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
		std::vector<unsigned char> data;
		std::vector<unsigned char *> rows;
		size_t rowStride;
		bool hasAlpha;

		// Properties
		unsigned int width;
		unsigned int height;
		int bitDepth;
		int colorType;
		int interlaceMethod;

		// Palette
		png_color *palette;
		int paletteSize;
		unsigned char *transPalette;
		int transPaletteSize;
		png_color_16 *transColor;
	};
}

anim::ReadInfo::ReadInfo(Image *image, const unsigned char *bytes, size_t size, std::string &errorText)
	: image(image)
	, png(nullptr)
	, info(nullptr)
	, endInfo(nullptr)
	, errorText(errorText)
	, readPos(bytes)
	, endPos(bytes + size)
	, rowStride(0)
	, hasAlpha(false)
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
	bool has_tRNS = ::png_get_tRNS(this->png, this->info, &this->transPalette, &this->transPaletteSize, &this->transColor) != 0;
	this->hasAlpha = has_tRNS || (this->colorType & PNG_COLOR_MASK_ALPHA) == PNG_COLOR_MASK_ALPHA;

	if (this->colorType == PNG_COLOR_TYPE_GRAY || this->colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		::png_set_gray_to_rgb(this->png);
	}

	if (this->hasAlpha)
	{
		::png_set_swap_alpha(this->png);
	}
	else
	{
		::png_set_add_alpha(this->png, 0xFFFF, PNG_FILLER_BEFORE);
	}

	::png_set_bgr(this->png);
	::png_set_expand_16(this->png);

	this->rowStride = 8 * this->width; // always AABBGGRR
	this->data.resize(this->rowStride * this->height);
	this->rows.resize(this->height);

	for (unsigned int i = 0; i < this->height; i++)
	{
		this->rows[i] = this->data.data() + this->rowStride * i;
	}

	::png_read_image(this->png, this->rows.data());
	::png_read_end(this->png, this->endInfo);

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
	size = std::min<size_t>(size, info.endPos - info.readPos);
	::memcpy(data, info.readPos, size);
	info.readPos += size;
}

void anim::Image::OnPngReadStatus(ReadInfo &info, unsigned int row, int pass)
{
}
