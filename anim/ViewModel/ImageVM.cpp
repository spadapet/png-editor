#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/Image.h"
#include "Model/RasterLayer.h"
#include "ViewModel/ImageVM.h"
#include "ViewModel/RasterLayerVM.h"

anim::ImageVM::ImageVM(std::shared_ptr<Image> image)
	: image(image)
	, layers(ref new Platform::Collections::Vector<ILayerVM ^>())
{
	Platform::WeakReference weakOwner(this);

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
	if (this->image != nullptr)
	{
		this->image->PropertyChanged.Remove(this->imageChangedCookie);
		this->image->LayerAdded.Remove(this->layerAddedCookie);
		this->image->LayerRemoved.Remove(this->layerRemovedCookie);
	}
}

unsigned int anim::ImageVM::Width::get()
{
	return (unsigned int)this->image->GetWidth();
}

unsigned int anim::ImageVM::Height::get()
{
	return (unsigned int)this->image->GetHeight();
}

double anim::ImageVM::WidthD::get()
{
	return (double)this->image->GetWidth();
}

double anim::ImageVM::HeightD::get()
{
	return (double)this->image->GetHeight();
}

Windows::UI::Xaml::Media::ImageSource ^anim::ImageVM::Source::get()
{
	if (this->imageSource == nullptr && this->image->GetGraph()->IsValid())
	{
		this->imageSource = ref new Windows::UI::Xaml::Media::Imaging::VirtualSurfaceImageSource(
			(int)this->image->GetWidth(), (int)this->image->GetHeight(), false);

		if (SUCCEEDED(((IUnknown *)this->imageSource)->QueryInterface(__uuidof(IVirtualSurfaceImageSourceNative), &this->imageSourceNative)))
		{
			this->imageSourceNative->SetDevice(this->image->GetGraph()->GetDevice());
		}
		else
		{
			this->imageSource = nullptr;
		}

		// this->imageSourceNative->RegisterForUpdatesNeeded(IVirtualSurfaceUpdatesCallbackNative
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
