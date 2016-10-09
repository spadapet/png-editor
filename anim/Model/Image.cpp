#include "pch.h"
#include "Core/PngReadInfo.h"
#include "Core/String.h"
#include "Model/Image.h"
#include "Model/RasterLayer.h"

anim::Image::Image()
	: width(0)
	, height(0)
{
}

anim::Image::~Image()
{
}

bool anim::Image::Initialize(const void *bytes, size_t byteSize, std::string &errorText)
{
	PngReadInfo info((const unsigned char *)bytes, byteSize);

	if (!info.Read())
	{
		errorText = info.GetErrorText().empty()
			? anim::Resource::GetStdString("ErrorInvalidPng")
			: info.GetErrorText();
		return false;
	}

	this->width = info.GetWidth();
	this->height = info.GetHeight();

	if (this->width == 0 || this->height == 0)
	{
		errorText = anim::Resource::GetStdString("ErrorImageSize");
		return false;
	}

	std::shared_ptr<Layer> layer = std::make_shared<RasterLayer>(info.GetWidth(), info.GetHeight(), info.TakeData());
	this->AddLayer(layer, nullptr);

	return true;
}

const std::vector<std::shared_ptr<anim::Layer>> &anim::Image::GetLayers() const
{
	return this->layers;
}

void anim::Image::AddLayer(std::shared_ptr<Layer> layer, std::shared_ptr<Layer> aboveLayer)
{
	if (layer != nullptr)
	{
		auto iter = std::find(this->layers.begin(), this->layers.end(), aboveLayer);
		if (iter != this->layers.end())
		{
			iter++;
		}

		iter = this->layers.insert(iter, layer);
		this->LayerAdded.Notify(layer, iter - this->layers.begin());
		this->PropertyChanged.Notify("Layers");
	}
}

void anim::Image::RemoveLayer(std::shared_ptr<Layer> layer)
{
	for (size_t i = 0; i < this->layers.size(); i++)
	{
		if (this->layers[i] == layer)
		{
			this->layers.erase(this->layers.begin() + i);
			this->LayerRemoved.Notify(layer, i);
			this->PropertyChanged.Notify("Layers");
		}
	}
}
