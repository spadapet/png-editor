#pragma once

namespace anim
{
	class Image
	{
	public:
		Image();
		~Image();

		bool Initialize(const void *bytes, size_t byteSize, std::string &errorText); // raw PNG file bytes

	private:
		static void PngErrorCallback(png_struct *pngStruct, const char *text);
		static void PngWarningCallback(png_struct *pngStruct, const char *text);
		static int PngUnknownChunkCallback(png_struct *pngStruct, png_unknown_chunk *chunk);

		void OnPngError(png_struct *pngStruct, const char *text);
		void OnPngWarning(png_struct *pngStruct, const char *text);
		int OnPngUnknownChunk(png_struct *pngStruct, png_unknown_chunk *chunk);

		png_struct *pngStruct;
		png_info *pngInfo;
		png_info *pngEndInfo;
	};
}
