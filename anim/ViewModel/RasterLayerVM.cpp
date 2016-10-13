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
}

anim::RasterLayerVM::~RasterLayerVM()
{
	this->Destroy();
}

void anim::RasterLayerVM::Destroy()
{
	if (this->layer != nullptr)
	{
		this->layer->PropertyChanged.Remove(this->layerChangedCookie);
		this->layer = nullptr;

		this->NotifyPropertyChanged();
	}
}

void anim::RasterLayerVM::Draw()
{
}

void anim::RasterLayerVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::RasterLayerVM::LayerPropertyChanged(const char *name)
{
}
