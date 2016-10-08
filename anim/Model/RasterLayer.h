#pragma once

#include "Core/PngReadInfo.h"
#include "Model/Layer.h"

namespace anim
{
	class RasterLayer : public Layer
	{
	public:
		RasterLayer(size_t width, size_t height, std::vector<PngColor> &&data);
		virtual ~RasterLayer() override;

		size_t GetWidth() const;
		size_t GetHeight() const;
		const PngColor *GetRow(size_t row) const;

	private:
		size_t width;
		size_t height;
		std::vector<PngColor> data;
	};
}
