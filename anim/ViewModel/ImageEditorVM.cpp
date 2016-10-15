#include "pch.h"
#include "ViewModel/ImageEditorVM.h"
#include "ViewModel/ImageVM.h"

class ImageCallback : public IVirtualSurfaceUpdatesCallbackNative
{
public:
	ImageCallback(anim::ImageVM ^owner);
	~ImageCallback();

	virtual HRESULT STDMETHODCALLTYPE UpdatesNeeded() override;
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override;
	virtual ULONG STDMETHODCALLTYPE AddRef() override;
	virtual ULONG STDMETHODCALLTYPE Release() override;

private:
	long refs;
	Platform::WeakReference weakOwner;
};

ImageCallback::ImageCallback(anim::ImageVM ^owner)
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
	anim::ImageVM ^owner = this->weakOwner.Resolve<anim::ImageVM>();
	if (owner != nullptr)
	{
		owner->ImageSourceUpdatesNeeded();
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
{
	this->imageSourceCallback = new ImageCallback(this);
}

anim::ImageEditorVM::ImageEditorVM()
	: ImageEditorVM(ref new ImageVM())
{
}

anim::ImageEditorVM::~ImageEditorVM()
{
}

void anim::ImageEditorVM::Destroy()
{
	if (this->image != nullptr)
	{
		this->image = nullptr;
		this->imageSource = nullptr;
		this->imageSourceNative.Reset();

		this->NotifyPropertyChanged();
	}
}

anim::ImageVM ^anim::ImageEditorVM::Image::get()
{
	return this->image;
}

Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^anim::ImageEditorVM::CreateImageSource(IVirtualSurfaceImageSourceNative **outNative)
{
	if (this->image == nullptr || !this->image->GetGraph()->IsValid())
	{
		return nullptr;
	}

	Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^imageSource =
		ref new Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource(
		(int)this->image->GetWidth(),
			(int)this->image->GetHeight(),
			false); // not opaque

	IUnknown *unknownSource = (IUnknown *)imageSource;
	ComPtr<IVirtualSurfaceImageSourceNative> imageSourceNative;

	if (FAILED(unknownSource->QueryInterface(__uuidof(IVirtualSurfaceImageSourceNative), &imageSourceNative)) ||
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

void anim::ImageEditorVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
