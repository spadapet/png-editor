#pragma once

namespace anim
{
	struct PngColor
	{
		unsigned short a;
		unsigned short b;
		unsigned short g;
		unsigned short r;
	};

	class PngReadInfo
	{
	public:
		PngReadInfo(const unsigned char *bytes, size_t size);
		~PngReadInfo();

		bool Read();

		size_t GetWidth() const;
		size_t GetHeight() const;
		const PngColor *GetRow(size_t row) const;
		std::string GetErrorText() const;
		std::vector<PngColor> TakeData();

	private:
		// Data
		png_struct *png;
		png_info *info;
		png_info *endInfo;
		std::string errorText;

		// Reading
		const unsigned char *readPos;
		const unsigned char *endPos;
		std::vector<PngColor> data;
		std::vector<PngColor *> rows;
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

	private:
		bool InternalRead();

		static void PngErrorCallback(png_struct *png, const char *text);
		static void PngWarningCallback(png_struct *png, const char *text);
		static int PngUnknownChunkCallback(png_struct *png, png_unknown_chunk *chunk);
		static void PngReadCallback(png_struct *png, unsigned char *data, size_t size);
		static void PngReadStatusCallback(png_struct *png, unsigned int row, int pass);

		void OnPngError(const char *text);
		void OnPngWarning(const char *text);
		int OnPngUnknownChunk(png_unknown_chunk *chunk);
		void OnPngRead(unsigned char *data, size_t size);
		void OnPngReadStatus(unsigned int row, int pass);
	};
}
