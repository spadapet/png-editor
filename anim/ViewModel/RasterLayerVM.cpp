#include "pch.h"
#include "Model/RasterLayer.h"
#include "ViewModel/RasterLayerVM.h"

anim::RasterLayerVM::RasterLayerVM(std::shared_ptr<RasterLayer> layer)
	: layer(layer)
{
	if (this->layer->GetWidth() && this->layer->GetHeight())
	{
		Microsoft::Graphics::Canvas::CanvasDevice ^device = Microsoft::Graphics::Canvas::CanvasDevice::GetSharedDevice();

		Platform::ArrayReference<unsigned char> bytes(
			(unsigned char *)layer->GetRow(0),
			layer->GetWidth() * layer->GetHeight() * 8);

		this->bitmap = Microsoft::Graphics::Canvas::CanvasBitmap::CreateFromBytes(
			device, bytes, layer->GetWidth(), layer->GetHeight(),
			Windows::Graphics::DirectX::DirectXPixelFormat::R16G16B16A16Int);
	}
}

anim::RasterLayerVM::~RasterLayerVM()
{
}

Microsoft::Graphics::Canvas::CanvasBitmap ^anim::RasterLayerVM::Bitmap::get()
{
	return this->bitmap;
}

void anim::RasterLayerVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
