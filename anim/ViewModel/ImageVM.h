#pragma once

#include "Core/Event.h"

namespace anim
{
	class Image;
	class Layer;
	interface class ILayerVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImageVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ImageVM(std::shared_ptr<Image> image);

	public:
		virtual ~ImageVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

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

		void ResetLayers();
		ILayerVM ^CreateLayer(std::shared_ptr<Layer> layer);

		std::shared_ptr<Image> image;
		Platform::Collections::Vector<ILayerVM ^> ^layers;
		Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^imageSource;
		ComPtr<IVirtualSurfaceImageSourceNative> imageSourceNative;
		EventCookie imageChangedCookie;
		EventCookie layerAddedCookie;
		EventCookie layerRemovedCookie;
	};
}
