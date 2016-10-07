#pragma once

#include "Core/PngReadInfo.h"
#include "Model/Layer.h"

namespace anim
{
	class RasterLayer : public Layer
	{
	public:
		RasterLayer(size_t width, size_t height, std::vector<PngColor> &&data);
		~RasterLayer();

	private:
		size_t width;
		size_t height;
		std::vector<PngColor> data;
	};
}
