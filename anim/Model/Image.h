#pragma once

#include "Core/Event.h"

namespace anim
{
	class GraphDevice;
	class Layer;

	class Image
	{
	public:
		Image(std::shared_ptr<GraphDevice> graph);
		~Image();

		ChangedEvent PropertyChanged;
		Event<std::shared_ptr<Layer>, size_t> LayerAdded;
		Event<std::shared_ptr<Layer>, size_t> LayerRemoved;

		bool Initialize(const void *bytes, size_t byteSize, std::string &errorText);

		// Properties
		size_t GetWidth() const;
		size_t GetHeight() const;
		std::shared_ptr<GraphDevice> GetGraph() const;

		// Layers
		const std::vector<std::shared_ptr<Layer>> &GetLayers() const;
		void AddLayer(std::shared_ptr<Layer> layer, std::shared_ptr<Layer> aboveLayer);
		void RemoveLayer(std::shared_ptr<Layer> layer);

	private:
		std::shared_ptr<GraphDevice> graph;
		std::vector<std::shared_ptr<Layer>> layers;
		size_t width;
		size_t height;
	};
}
