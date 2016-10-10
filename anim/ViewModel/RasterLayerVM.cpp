#include "pch.h"
#include "Model/RasterLayer.h"
#include "ViewModel/RasterLayerVM.h"

anim::RasterLayerVM::RasterLayerVM(std::shared_ptr<RasterLayer> layer)
	: layer(layer)
{
	Platform::WeakReference weakOwner(this);

	this->layerChangedCookie = this->layer->PropertyChanged.Add([weakOwner](const char *name)
	{
		RasterLayerVM ^owner = weakOwner.Resolve<RasterLayerVM>();
		if (owner != nullptr)
		{
			owner->LayerPropertyChanged(name);
		}
	});

	Microsoft::Graphics::Canvas::CanvasDevice ^device = Microsoft::Graphics::Canvas::CanvasDevice::GetSharedDevice();

	Platform::ArrayReference<unsigned char> bytes(
		(unsigned char *)layer->GetRow(0),
		(unsigned int)(layer->GetWidth() * layer->GetHeight() * 8));

	this->bitmap = Microsoft::Graphics::Canvas::CanvasBitmap::CreateFromBytes(
		device, bytes, (unsigned int)layer->GetWidth(), (unsigned int)layer->GetHeight(),
		Windows::Graphics::DirectX::DirectXPixelFormat::R16G16B16A16UIntNormalized,
		96, // dpi
		Microsoft::Graphics::Canvas::CanvasAlphaMode::Premultiplied); // Straight);
}

anim::RasterLayerVM::~RasterLayerVM()
{
}

Microsoft::Graphics::Canvas::CanvasBitmap ^anim::RasterLayerVM::Bitmap::get()
{
	return this->bitmap;
}

void anim::RasterLayerVM::Draw(Microsoft::Graphics::Canvas::CanvasDrawingSession ^session, Windows::Foundation::Rect rect)
{
	if (this->bitmap != nullptr)
	{
		session->DrawImage(this->bitmap);
	}
}

void anim::RasterLayerVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::RasterLayerVM::LayerPropertyChanged(const char *name)
{
}
