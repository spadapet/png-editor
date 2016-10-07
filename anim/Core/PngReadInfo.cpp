#include "pch.h"
#include "Core/PngReadInfo.h"
#include "Core/String.h"

anim::PngReadInfo::PngReadInfo(const unsigned char *bytes, size_t size)
	: png(nullptr)
	, info(nullptr)
	, endInfo(nullptr)
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
	this->png = ::png_create_read_struct(PNG_LIBPNG_VER_STRING, this, &PngReadInfo::PngErrorCallback, &PngReadInfo::PngWarningCallback);
	this->info = ::png_create_info_struct(this->png);
	this->endInfo = ::png_create_info_struct(this->png);
}

anim::PngReadInfo::~PngReadInfo()
{
	::png_destroy_read_struct(&this->png, &this->info, &this->endInfo);
}

bool anim::PngReadInfo::Read()
{
	bool status = true;

	try
	{
		if (!this->InternalRead())
		{
			if (this->errorText.empty())
			{
				this->errorText = anim::ConvertString(anim::Resource::GetString("ErrorInvalidPng"));
			}

			status = false;
		}
	}
	catch (Platform::Exception ^exception)
	{
		this->errorText = anim::ConvertString(exception->Message);
		status = false;
	}

	return status;
}

size_t anim::PngReadInfo::GetWidth() const
{
	return this->width;
}

size_t anim::PngReadInfo::GetHeight() const
{
	return this->height;
}

const short int *anim::PngReadInfo::GetRow(size_t row) const
{
	return (const short int *)this->rows[row];
}

std::string anim::PngReadInfo::GetErrorText() const
{
	return this->errorText;
}

bool anim::PngReadInfo::InternalRead()
{
	if (::png_sig_cmp(this->readPos, 0, this->endPos - this->readPos) != 0)
	{
		return false;
	}

	::png_set_chunk_malloc_max(this->png, 0x10000000);
	::png_set_read_fn(this->png, this, &PngReadInfo::PngReadCallback);
	::png_set_read_user_chunk_fn(this->png, this, &PngReadInfo::PngUnknownChunkCallback);
	::png_set_read_status_fn(this->png, &PngReadInfo::PngReadStatusCallback);
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

void anim::PngReadInfo::PngErrorCallback(png_struct *png, const char *text)
{
	PngReadInfo *info = (PngReadInfo *)::png_get_error_ptr(png);
	info->OnPngError(text);
}

void anim::PngReadInfo::PngWarningCallback(png_struct *png, const char *text)
{
	PngReadInfo *info = (PngReadInfo *)::png_get_error_ptr(png);
	info->OnPngWarning(text);
}

int anim::PngReadInfo::PngUnknownChunkCallback(png_struct *png, png_unknown_chunk *chunk)
{
	PngReadInfo *info = (PngReadInfo *)::png_get_user_chunk_ptr(png);
	return info->OnPngUnknownChunk(chunk);
}

void anim::PngReadInfo::PngReadCallback(png_struct *png, unsigned char *data, size_t size)
{
	PngReadInfo *info = (PngReadInfo *)::png_get_io_ptr(png);
	info->OnPngRead(data, size);
}

void anim::PngReadInfo::PngReadStatusCallback(png_struct *png, unsigned int row, int pass)
{
	PngReadInfo *info = (PngReadInfo *)::png_get_io_ptr(png);
	info->OnPngReadStatus(row, pass);
}

void anim::PngReadInfo::OnPngError(const char *text)
{
	throw ref new Platform::Exception(E_FAIL, anim::Resource::GetString("ErrorFailedPng"));
}

void anim::PngReadInfo::OnPngWarning(const char *text)
{
}

int anim::PngReadInfo::OnPngUnknownChunk(png_unknown_chunk *chunk)
{
	return 0;
}

void anim::PngReadInfo::OnPngRead(unsigned char *data, size_t size)
{
	size = std::min<size_t>(size, this->endPos - this->readPos);
	::memcpy(data, this->readPos, size);
	this->readPos += size;
}

void anim::PngReadInfo::OnPngReadStatus(unsigned int row, int pass)
{
}
