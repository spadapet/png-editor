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

size_t anim::RasterLayer::GetWidth() const
{
	return this->width;
}

size_t anim::RasterLayer::GetHeight() const
{
	return this->height;
}

const anim::PngColor *anim::RasterLayer::GetRow(size_t row) const
{
	return &this->data[this->width * row];
}
