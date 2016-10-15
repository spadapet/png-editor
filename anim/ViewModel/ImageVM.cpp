#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/Image.h"
#include "Model/RasterLayer.h"
#include "ViewModel/ImageVM.h"
#include "ViewModel/RasterLayerVM.h"

anim::ImageVM::ImageVM(std::shared_ptr<Image> image, bool active)
	: image(image)
	, graph(image->GetGraph())
	, layers(ref new Platform::Collections::Vector<ILayerVM ^>())
	, active(false)
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

	this->graphResetCookie = this->graph->DeviceReset.Add([weakOwner]()
	{
		ImageVM ^owner = weakOwner.Resolve<ImageVM>();
		if (owner != nullptr)
		{
			owner->GraphDeviceReset();
		}
	});

	this->ResetLayers();
}

anim::ImageVM::ImageVM()
	: ImageVM(std::make_shared<Image>(std::make_shared<GraphDevice>()), true)
{
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
	}
	if (this->graph != nullptr)
	{
		this->graph->DeviceReset.Remove(this->graphResetCookie);
		this->graph = nullptr;
	}

	this->NotifyPropertyChanged();
}

void anim::ImageVM::ImageSourceUpdatesNeeded()
{
	if (this->imageSourceNative == nullptr)
	{
		assert(false);
		return;
	}

	// this->imageSourceNative->
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

void anim::ImageVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ImageVM::ImagePropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);
	bool newImage = allChanged;
	bool newSize = allChanged;

	if (allChanged || strcmp(name, "Layers") == 0)
	{
		this->ResetLayers();
	}

	if (allChanged || strcmp(name, "Width") == 0)
	{
		this->NotifyPropertyChanged("Width");
		this->NotifyPropertyChanged("WidthD");
		newSize = true;
	}

	if (allChanged || strcmp(name, "Height") == 0)
	{
		this->NotifyPropertyChanged("Height");
		this->NotifyPropertyChanged("HeightD");
		newSize = true;
	}

	if (newImage)
	{
		this->imageSourceNative.Reset();
		this->imageSource = nullptr;
		this->NotifyPropertyChanged("Source");
	}
	else if (newSize && this->imageSourceNative != nullptr)
	{
		this->imageSourceNative->Resize((int)this->Width, (int)this->Height);
	}
}

void anim::ImageVM::ImageLayerAdded(std::shared_ptr<Layer> layer, size_t index)
{
	this->layers->InsertAt((unsigned int)index, this->CreateLayer(layer));
}

void anim::ImageVM::ImageLayerRemoved(std::shared_ptr<Layer> layer, size_t index)
{
	ILayerVM ^layerVM = this->layers->GetAt((unsigned int)index);
	this->layers->RemoveAt((unsigned int)index);
	layerVM->Destroy();
}

void anim::ImageVM::GraphDeviceReset()
{
	// This will recreate all graphic device stuff
	this->ImagePropertyChanged(nullptr);
}

void anim::ImageVM::ResetLayers()
{
	std::vector<ILayerVM ^> layers;

	for (ILayerVM ^layer : this->layers)
	{
		layer->Destroy();
	}

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

	// Unknown type of layer
	assert(false);
	return nullptr;
}
