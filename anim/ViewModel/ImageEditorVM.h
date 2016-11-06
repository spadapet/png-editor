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
		void ImageDamaged(RectInt rect);
		void GraphDeviceReset();
		void Render(const RectInt &imageRect, const RectInt &pixelRect, ID3D11Texture2D *texture);

		// Image document
		ImageVM ^image;
		EventCookie imageDamagedCookie;
		Windows::Foundation::EventRegistrationToken imageChangedToken;

		// Image source for XAML
		Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^imageSource;
		ComPtr<IVirtualSurfaceImageSourceNative> imageSourceNative;
		ComPtr<IVirtualSurfaceUpdatesCallbackNative> imageSourceCallback;

		// Graphics
		std::shared_ptr<GraphDevice> graph;
		EventCookie graphResetCookie;
		ComPtr<ID3D11Texture2D1> scratchTexture;
		ComPtr<ID3D11RenderTargetView1> scratchRenderTarget;
	};
}
