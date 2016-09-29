#pragma once

namespace anim
{
	class Image
	{
	public:
		Image();
		Image(const void *bytes, size_t byteSize);
		~Image();
	};
}
