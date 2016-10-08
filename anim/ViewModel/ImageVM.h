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

		property Windows::Foundation::Collections::IVector<ILayerVM ^> ^Layers { Windows::Foundation::Collections::IVector<ILayerVM ^> ^get(); }
		property Windows::UI::Xaml::Interop::IBindableObservableVector ^BindableLayers { Windows::UI::Xaml::Interop::IBindableObservableVector ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ImagePropertyChanged(const char *name);
		void ImageLayerAdded(std::shared_ptr<Layer> layer, size_t index);
		void ImageLayerRemoved(std::shared_ptr<Layer> layer, size_t index);

		std::shared_ptr<Image> image;
		Platform::Collections::Vector<ILayerVM ^> ^layers;
		EventCookie imageChangedCookie;
		EventCookie layerAddedCookie;
		EventCookie layerRemovedCookie;
	};
}
