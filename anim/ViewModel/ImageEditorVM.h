#pragma once

#include "Core/Event.h"

namespace anim
{
	class GraphDevice;
	ref class ImageVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImageEditorVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ImageEditorVM(ImageVM ^image);

	public:
		ImageEditorVM();
		virtual ~ImageEditorVM();

		void Destroy();
		void ImageSourceUpdatesNeeded();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property ImageVM ^Image { ImageVM ^get(); }
		property Windows::UI::Xaml::Media::ImageSource ^Source { Windows::UI::Xaml::Media::ImageSource ^get(); }

	private:
		Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^CreateImageSource(IVirtualSurfaceImageSourceNative **outNative);
		void GraphDeviceReset();

		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		ImageVM ^image;
		Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^imageSource;
		ComPtr<IVirtualSurfaceImageSourceNative> imageSourceNative;
		ComPtr<IVirtualSurfaceUpdatesCallbackNative> imageSourceCallback;
		Windows::Foundation::EventRegistrationToken imageChangedToken;
		std::shared_ptr<GraphDevice> graph;
		EventCookie graphResetCookie;
	};
}
