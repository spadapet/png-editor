#pragma once

#include "Core/Event.h"

namespace anim
{
	class GraphDevice;
	class Image;
	class Layer;
	interface class ILayerVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImageVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ImageVM(std::shared_ptr<Image> image, bool active);

	public:
		ImageVM();
		virtual ~ImageVM();

		void Destroy();
		void ImageSourceUpdatesNeeded();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		property bool IsActive { bool get(); void set(bool value); }
		property unsigned int Width { unsigned int get(); }
		property unsigned int Height { unsigned int get(); }
		property double WidthD { double get(); }
		property double HeightD { double get(); }
		property Windows::UI::Xaml::Media::ImageSource ^Source { Windows::UI::Xaml::Media::ImageSource ^get(); }
		property Windows::Foundation::Collections::IVector<ILayerVM ^> ^Layers { Windows::Foundation::Collections::IVector<ILayerVM ^> ^get(); }
		property Windows::UI::Xaml::Interop::IBindableObservableVector ^BindableLayers { Windows::UI::Xaml::Interop::IBindableObservableVector ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ImagePropertyChanged(const char *name);
		void ImageLayerAdded(std::shared_ptr<Layer> layer, size_t index);
		void ImageLayerRemoved(std::shared_ptr<Layer> layer, size_t index);
		void GraphDeviceReset();

		void ResetLayers();
		ILayerVM ^CreateLayer(std::shared_ptr<Layer> layer);

		std::shared_ptr<Image> image;
		std::shared_ptr<GraphDevice> graph;
		Platform::Collections::Vector<ILayerVM ^> ^layers;
		EventCookie imageChangedCookie;
		EventCookie layerAddedCookie;
		EventCookie layerRemovedCookie;
		EventCookie graphResetCookie;
		bool active;
	};
}
