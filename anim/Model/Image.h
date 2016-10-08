#pragma once

#include "Core/Event.h"

namespace anim
{
	class Layer;

	class Image
	{
	public:
		Image();
		~Image();

		ChangedEvent PropertyChanged;
		Event<std::shared_ptr<Layer>, size_t> LayerAdded;
		Event<std::shared_ptr<Layer>, size_t> LayerRemoved;

		bool Initialize(const void *bytes, size_t byteSize, std::string &errorText);

		// Layers
		const std::vector<std::shared_ptr<Layer>> &GetLayers() const;
		void AddLayer(std::shared_ptr<Layer> layer, std::shared_ptr<Layer> aboveLayer);
		void RemoveLayer(std::shared_ptr<Layer> layer);

	private:
		std::vector<std::shared_ptr<Layer>> layers;
	};
}
