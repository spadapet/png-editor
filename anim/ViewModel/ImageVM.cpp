#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/Image.h"
#include "Model/RasterLayer.h"
#include "ViewModel/ImageVM.h"
#include "ViewModel/RasterLayerVM.h"

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

anim::ImageVM::ImageVM(std::shared_ptr<Image> image)
	: image(image)
	, layers(ref new Platform::Collections::Vector<ILayerVM ^>())
	, active(false)
{
	Platform::WeakReference weakOwner(this);

	this->imageSourceCallback = new ImageCallback(this);

	this->imageChangedCookie = this->image->PropertyChanged.Add([weakOwner](const char *name)
	{
		ImageVM ^owner = weakOwner.Resolve<ImageVM>();
		if (owner != nullptr)
		{
			owner->ImagePropertyChanged(name);
		}
	});

	this->layerAddedCookie = this->image->LayerAdded.Add([weakOwner](std::shared_ptr<Layer> layer, size_t index)
	{
		ImageVM ^owner = weakOwner.Resolve<ImageVM>();
		if (owner != nullptr)
		{
			owner->ImageLayerAdded(layer, index);
		}
	});

	this->layerRemovedCookie = this->image->LayerRemoved.Add([weakOwner](std::shared_ptr<Layer> layer, size_t index)
	{
		ImageVM ^owner = weakOwner.Resolve<ImageVM>();
		if (owner != nullptr)
		{
			owner->ImageLayerRemoved(layer, index);
		}
	});

	this->ResetLayers();
}

anim::ImageVM::~ImageVM()
{
	this->Destroy();
}

void anim::ImageVM::Destroy()
{
	for (ILayerVM ^layer : this->layers)
	{
		layer->Destroy();
	}

	this->layers->Clear();

	if (this->image != nullptr)
	{
		this->image->PropertyChanged.Remove(this->imageChangedCookie);
		this->image->LayerAdded.Remove(this->layerAddedCookie);
		this->image->LayerRemoved.Remove(this->layerRemovedCookie);
		this->image = nullptr;

		this->active = false;
		this->imageSource = nullptr;
		this->imageSourceNative.Reset();

		this->NotifyPropertyChanged();
	}
}

void anim::ImageVM::ImageSourceUpdatesNeeded()
{
}

bool anim::ImageVM::IsActive::get()
{
	return this->active;
}

void anim::ImageVM::IsActive::set(bool value)
{
	if (this->active != value)
	{
		this->active = value;
		this->NotifyPropertyChanged("IsActive");
	}
}

unsigned int anim::ImageVM::Width::get()
{
	return (this->image != nullptr) ? (unsigned int)this->image->GetWidth() : 0;
}

unsigned int anim::ImageVM::Height::get()
{
	return (this->image != nullptr) ? (unsigned int)this->image->GetHeight() : 0;
}

double anim::ImageVM::WidthD::get()
{
	return (double)this->Width;
}

double anim::ImageVM::HeightD::get()
{
	return (double)this->Height;
}

Windows::UI::Xaml::Media::ImageSource ^anim::ImageVM::Source::get()
{
	if (this->imageSource == nullptr)
	{
		this->imageSource = this->CreateImageSource(&this->imageSourceNative);
	}

	return this->imageSource;
}

Windows::Foundation::Collections::IVector<anim::ILayerVM ^> ^anim::ImageVM::Layers::get()
{
	return this->layers;
}

Windows::UI::Xaml::Interop::IBindableObservableVector ^anim::ImageVM::BindableLayers::get()
{
	return this->layers;
}

Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^anim::ImageVM::CreateImageSource(IVirtualSurfaceImageSourceNative **outNative)
{
	if (this->image == nullptr || !this->image->GetGraph()->IsValid())
	{
		return nullptr;
	}

	Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource ^imageSource =
		ref new Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource(
			(int)this->image->GetWidth(),
			(int)this->image->GetHeight(),
			false); // opaque

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

void anim::ImageVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ImageVM::ImagePropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);
	bool newImage = false;

	if (allChanged || strcmp(name, "Layers") == 0)
	{
		this->ResetLayers();
	}

	if (allChanged || strcmp(name, "Width") == 0)
	{
		this->NotifyPropertyChanged("Width");
		this->NotifyPropertyChanged("WidthD");
		newImage = true;
	}

	if (allChanged || strcmp(name, "Height") == 0)
	{
		this->NotifyPropertyChanged("Height");
		this->NotifyPropertyChanged("HeightD");
		newImage = true;
	}

	if (newImage)
	{
		this->imageSourceNative.Reset();
		this->imageSource = nullptr;
		this->NotifyPropertyChanged("Source");
	}
}

void anim::ImageVM::ImageLayerAdded(std::shared_ptr<Layer> layer, size_t index)
{
	this->layers->InsertAt((unsigned int)index, this->CreateLayer(layer));
}

void anim::ImageVM::ImageLayerRemoved(std::shared_ptr<Layer> layer, size_t index)
{
	this->layers->RemoveAt((unsigned int)index);
}

void anim::ImageVM::ResetLayers()
{
	std::vector<ILayerVM ^> layers;

	for (std::shared_ptr<Layer> layer : this->image->GetLayers())
	{
		layers.push_back(this->CreateLayer(layer));
	}

	this->layers->ReplaceAll(Platform::ArrayReference<ILayerVM ^>(layers.data(), (unsigned int)layers.size()));
}

anim::ILayerVM ^anim::ImageVM::CreateLayer(std::shared_ptr<Layer> layer)
{
	std::shared_ptr<RasterLayer> rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer);
	if (rasterLayer != nullptr)
	{
		return ref new RasterLayerVM(rasterLayer);
	}

	assert(false);
	return nullptr;
}
