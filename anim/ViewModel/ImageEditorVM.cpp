#include "pch.h"
#include "ViewModel/ImageEditorVM.h"
#include "ViewModel/ImageVM.h"

anim::ImageEditorVM::ImageEditorVM(ImageVM ^image)
	: image(image)
{
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
		this->NotifyPropertyChanged();
	}
}

anim::ImageVM ^anim::ImageEditorVM::Image::get()
{
	return this->image;
}

void anim::ImageEditorVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
