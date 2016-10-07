#include "pch.h"
#include "Model/RasterLayer.h"

anim::RasterLayer::RasterLayer(size_t width, size_t height, std::vector<PngColor> &&data)
	: width(width)
	, height(height)
	, data(std::move(data))
{
}

anim::RasterLayer::~RasterLayer()
{
}
