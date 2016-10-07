#pragma once

namespace anim
{
	class Image
	{
	public:
		Image();
		~Image();

		bool Initialize(const void *bytes, size_t byteSize, std::string &errorText); // raw PNG file bytes
	};
}
