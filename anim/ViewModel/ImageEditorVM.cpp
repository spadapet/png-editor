#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/AppState.h"
#include "Model/Image.h"
#include "ViewModel/ImageEditorVM.h"
#include "ViewModel/ImageVM.h"

class ImageCallback : public IVirtualSurfaceUpdatesCallbackNative
{
public:
	ImageCallback(anim::ImageEditorVM ^owner);
	~ImageCallback();

	virtual HRESULT STDMETHODCALLTYPE UpdatesNeeded() override;
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;
	virtual ULONG STDMETHODCALLTYPE AddRef() override;
	virtual ULONG STDMETHODCALLTYPE Release() override;

private:
	long refs;
	Platform::WeakReference weakOwner;
};

ImageCallback::ImageCallback(anim::ImageEditorVM ^owner)
	: weakOwner(owner)
	, refs(0)
{
}

ImageCallback::~ImageCallback()
{
	assert(this->refs == 0);
}

HRESULT ImageCallback::UpdatesNeeded()
{
	anim::ImageEditorVM ^owner = this->weakOwner.Resolve<anim::ImageEditorVM>();
	if (owner != nullptr)
	{
		owner->UpdateVirtualImage();
	}

	return S_OK;
}

HRESULT ImageCallback::QueryInterface(REFIID riid, void **ppvObject)
{
	if (ppvObject != nullptr && (riid == IID_IUnknown || riid == __uuidof(IVirtualSurfaceUpdatesCallbackNative)))
	{
		this->AddRef();
		*ppvObject = (IVirtualSurfaceUpdatesCallbackNative *)this;
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG ImageCallback::AddRef()
{
	return ::InterlockedIncrement(&this->refs);
}

ULONG ImageCallback::Release()
{
	ULONG refs = ::InterlockedDecrement(&this->refs);
	if (refs == 0)
	{
		delete this;
	}

	return refs;
}

anim::ImageEditorVM::ImageEditorVM(ImageVM ^image)
	: image(image)
	, graph(image->GetImage()->GetGraph())
{
	Platform::WeakReference weakOwner(this);

	this->imageSourceCallback = new ImageCallback(this);

	this->imageChangedToken = this->image->PropertyChanged +=
		ref new Windows::UI::Xaml::Data::PropertyChangedEventHandler(this, &ImageEditorVM::ImagePropertyChanged);

	this->imageDamagedCookie = this->image->GetImage()->Damaged.Add([weakOwner](RectInt rect)
	{
		ImageEditorVM ^owner = weakOwner.Resolve<ImageEditorVM>();
		if (owner != nullptr)
		{
			owner->ImageDamaged(rect);
		}
	});

	this->graphResetCookie = this->graph->DeviceReset.Add([weakOwner]()
	{
		ImageEditorVM ^owner = weakOwner.Resolve<ImageEditorVM>();
		if (owner != nullptr)
		{
			owner->GraphDeviceReset();
		}
	});
}

anim::ImageEditorVM::ImageEditorVM()
	: ImageEditorVM(ref new ImageVM())
{
}

anim::ImageEditorVM::~ImageEditorVM()
{
	this->Destroy();
}

void anim::ImageEditorVM::Destroy()
{
	if (this->image != nullptr)
	{
		this->image->PropertyChanged -= this->imageChangedToken;
		this->image = nullptr;
		this->imageSource = nullptr;
		this->imageSourceNative.Reset();
	}

	if (this->graph != nullptr)
	{
		this->graph->DeviceReset.Remove(this->graphResetCookie);
		this->graph = nullptr;
	}

	this->NotifyPropertyChanged();
}

void anim::ImageEditorVM::UpdateVirtualImage()
{
	if (this->imageSourceNative == nullptr)
	{
		assert(false);
		return;
	}

	DWORD count = 0;
	if (SUCCEEDED(this->imageSourceNative->GetUpdateRectCount(&count)))
	{
		std::vector<RECT> rects;
		rects.resize(count);

		RECT visibleRect;
		if (SUCCEEDED(this->imageSourceNative->GetUpdateRects(rects.data(), count)) &&
			SUCCEEDED(this->imageSourceNative->GetVisibleBounds(&visibleRect)))
		{
			RectFloat visibleRectF(visibleRect);
			std::shared_ptr<AppState> app = this->image->GetAppState();

			for (RECT imageRect : rects)
			{
				RectInt pixelRect = app->DipToPixel(RectFloat(imageRect)).ToInt();

				POINT offset{ 0, 0 };
				ComPtr<IDXGISurface> surface;
				if (this->imageSourceNative->BeginDraw(pixelRect.ToRECT(), &surface, &offset))
				{
					pixelRect.MoveTopLeft(offset.x, offset.y);

					ComPtr<ID3D11Texture2D> texture;
					if (SUCCEEDED(surface.As(&texture)))
					{
						this->Render(RectInt(imageRect), pixelRect, texture.Get());
					}

					this->imageSourceNative->EndDraw();
				}
				else
				{
					this->graph->ResetIfNeeded();
				}
			}
		}
	}
}

anim::ImageVM ^anim::ImageEditorVM::Image::get()
{
	return this->image;
}

double anim::ImageEditorVM::ImageWidth::get()
{
	return (this->image != nullptr) ? (double)this->image->Width : 0.0;
}

double anim::ImageEditorVM::ImageHeight::get()
{
	return (this->image != nullptr) ? (double)this->image->Height : 0.0;
}

Windows::UI::Xaml::Media::ImageSource ^anim::ImageEditorVM::ImageSource::get()
{
	if (this->imageSource == nullptr)
	{
		this->imageSource = this->CreateImageSource(&this->imageSourceNative);
	}

	return this->imageSource;
}

void anim::ImageEditorVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^anim::ImageEditorVM::CreateImageSource(
	IVirtualSurfaceImageSourceNative **outNative)
{
	if (this->image == nullptr || this->graph == nullptr || !this->graph->IsValid())
	{
		return nullptr;
	}

	Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^imageSource =
		ref new Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource(
		(int)this->image->Width,
		(int)this->image->Height,
		false); // not opaque

	IUnknown *unknownSource = (IUnknown *)imageSource;
	ComPtr<IVirtualSurfaceImageSourceNative> imageSourceNative;

	if (FAILED(unknownSource->QueryInterface(__uuidof(IVirtualSurfaceImageSourceNative), &imageSourceNative)) ||
		FAILED(imageSourceNative->SetDevice(this->graph->GetDevice())) ||
		FAILED(imageSourceNative->RegisterForUpdatesNeeded(this->imageSourceCallback.Get())))
	{
		return nullptr;
	}

	if (outNative != nullptr)
	{
		imageSourceNative.CopyTo(outNative);
	}

	return imageSource;
}

void anim::ImageEditorVM::ImagePropertyChanged(Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args)
{
	bool allChanged = args->PropertyName == nullptr || args->PropertyName->Length() == 0;
	bool sizeChanged = false;

	if (allChanged || args->PropertyName == "Width")
	{
		this->NotifyPropertyChanged("ImageWidth");
		sizeChanged = true;
	}

	if (allChanged || args->PropertyName == "Height")
	{
		this->NotifyPropertyChanged("ImageHeight");
		sizeChanged = true;
	}

	if (sizeChanged && this->imageSourceNative != nullptr)
	{
		std::shared_ptr<AppState> app = this->image->GetAppState();
		int pixelWidth = (int)this->image->Width;
		int pixelHeight = (int)this->image->Height;
		int dipWidth = (int)app->DipToPixel(pixelWidth);
		int dipHeight = (int)app->DipToPixel(pixelHeight);

		this->imageSourceNative->Resize(dipWidth, dipHeight);
		this->imageSourceNative->Invalidate(RECT{ 0, 0, pixelWidth, pixelHeight });
	}
}

void anim::ImageEditorVM::ImageDamaged(RectInt rect)
{
	if (this->imageSourceNative != nullptr)
	{
		this->imageSourceNative->Invalidate(rect.ToRECT());
	}
}

void anim::ImageEditorVM::GraphDeviceReset()
{
	this->imageSourceNative.Reset();
	this->imageSource = nullptr;
	this->NotifyPropertyChanged("Source");
}

void anim::ImageEditorVM::Render(const RectInt &imageRect, const RectInt &pixelRect, ID3D11Texture2D *texture)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	D3D11_RENDER_TARGET_VIEW_DESC1 viewDesc;
	::ZeroMemory(&viewDesc, sizeof(viewDesc));
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	ComPtr<ID3D11RenderTargetView1> view;
	if (SUCCEEDED(this->graph->GetDevice3d()->CreateRenderTargetView1(texture, &viewDesc, &view)))
	{
		// this->graph->GetContext2d()->Create
	}
}
