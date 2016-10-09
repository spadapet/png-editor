#include "pch.h"
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

	for (std::shared_ptr<Layer> layer : this->image->GetLayers())
	{
		this->ImageLayerAdded(layer, this->layers->Size);
	}
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
	}
}

void anim::ImageVM::ImageLayerAdded(std::shared_ptr<Layer> layer, size_t index)
{
	std::shared_ptr<RasterLayer> rasterLayer = std::dynamic_pointer_cast<RasterLayer>(layer);
	if (rasterLayer != nullptr)
	{
		this->layers->InsertAt((unsigned int)index, ref new RasterLayerVM(rasterLayer));
	}
	else
	{
		assert(false);
	}
}

void anim::ImageVM::ImageLayerRemoved(std::shared_ptr<Layer> layer, size_t index)
{
	this->layers->RemoveAt((unsigned int)index);
}
