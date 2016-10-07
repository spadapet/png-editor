#include "pch.h"
#include "ViewModel/LayerVM.h"

anim::RasterLayerVM::RasterLayerVM(std::shared_ptr<RasterLayer> layer)
	: layer(layer)
{
}

anim::RasterLayerVM::~RasterLayerVM()
{
}

void anim::RasterLayerVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
