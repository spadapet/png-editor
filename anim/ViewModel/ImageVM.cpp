#include "pch.h"
#include "ViewModel/ImageVM.h"

anim::ImageVM::ImageVM(std::shared_ptr<Image> image)
	: image(image)
{
}

anim::ImageVM::~ImageVM()
{
}

void anim::ImageVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
