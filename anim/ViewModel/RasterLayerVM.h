#pragma once

#include "Core/Event.h"
#include "ViewModel/ILayerVM.h"

namespace anim
{
	class RasterLayer;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class RasterLayerVM sealed : ILayerVM
	{
	internal:
		RasterLayerVM(std::shared_ptr<RasterLayer> layer);

	public:
		virtual ~RasterLayerVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		virtual void Destroy();
		virtual void Draw();

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void LayerPropertyChanged(const char *name);

		std::shared_ptr<RasterLayer> layer;
		EventCookie layerChangedCookie;
	};
}
