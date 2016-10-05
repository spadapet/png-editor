#pragma once

namespace anim
{
	struct ReadInfo;

	class Image
	{
	public:
		Image();
		~Image();

		bool Initialize(const void *bytes, size_t byteSize, std::string &errorText); // raw PNG file bytes

		static void PngErrorCallback(png_struct *png, const char *text);
		static void PngWarningCallback(png_struct *png, const char *text);
		static int PngUnknownChunkCallback(png_struct *png, png_unknown_chunk *chunk);
		static void PngReadCallback(png_struct *png, unsigned char *data, size_t size);
		static void PngReadStatusCallback(png_struct *png, unsigned int row, int pass);

	private:
		void OnPngError(ReadInfo &info, const char *text);
		void OnPngWarning(ReadInfo &info, const char *text);
		int OnPngUnknownChunk(ReadInfo &info, png_unknown_chunk *chunk);
		void OnPngRead(ReadInfo &info, unsigned char *data, size_t size);
		void OnPngReadStatus(ReadInfo &info, unsigned int row, int pass);
	};
}
