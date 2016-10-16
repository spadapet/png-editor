#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/Image.h"
#include "Model/RasterLayer.h"
#include "ViewModel/ImageVM.h"
#include "ViewModel/RasterLayerVM.h"

anim::ImageVM::ImageVM(std::shared_ptr<Image> image)
	: image(image)
	, graph(image->GetGraph())
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
	}

	if (this->graph != nullptr)
	{
		this->graph->DeviceReset.Remove(this->graphResetCookie);
		this->graph = nullptr;
	}

	this->NotifyPropertyChanged();
}

std::shared_ptr<anim::Image> anim::ImageVM::GetImage()
{
	return this->image;
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

	if (allChanged || strcmp(name, "Layers") == 0)
	{
		this->ResetLayers();
	}

	if (allChanged || strcmp(name, "Width") == 0)
	{
		this->NotifyPropertyChanged("Width");
		this->NotifyPropertyChanged("WidthD");
	}

	if (allChanged || strcmp(name, "Height") == 0)
	{
		this->NotifyPropertyChanged("Height");
		this->NotifyPropertyChanged("HeightD");
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
