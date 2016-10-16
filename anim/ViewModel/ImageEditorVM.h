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
	public:
		ImageEditorVM(ImageVM ^image);
		ImageEditorVM();
		virtual ~ImageEditorVM();

		void Destroy();
		void UpdateVirtualImage();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property ImageVM ^Image { ImageVM ^get(); }
		property double ImageWidth { double get(); }
		property double ImageHeight { double get(); }
		property Windows::UI::Xaml::Media::ImageSource ^ImageSource { Windows::UI::Xaml::Media::ImageSource ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^CreateImageSource(IVirtualSurfaceImageSourceNative **outNative);
		void ImagePropertyChanged(Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args);
		void ImageDamaged();
		void GraphDeviceReset();

		ImageVM ^image;
		Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^imageSource;
		ComPtr<IVirtualSurfaceImageSourceNative> imageSourceNative;
		ComPtr<IVirtualSurfaceUpdatesCallbackNative> imageSourceCallback;
		Windows::Foundation::EventRegistrationToken imageChangedToken;
		std::shared_ptr<GraphDevice> graph;
		EventCookie imageDamagedCookie;
		EventCookie graphResetCookie;
	};
}
