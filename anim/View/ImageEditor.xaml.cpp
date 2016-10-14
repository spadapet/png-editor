#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/Image.h"
#include "View/ImageEditor.xaml.h"
#include "ViewModel/ImageVM.h"

anim::ImageEditor::ImageEditor(ImageVM ^image)
	: image(image)
{
	this->imageChangedToken = this->image->PropertyChanged +=
		ref new Windows::UI::Xaml::Data::PropertyChangedEventHandler(
			this, &ImageEditor::OnImagePropertyChanged);

	this->DataContext = image;
	this->InitializeComponent();
}

anim::ImageEditor::ImageEditor()
	: ImageEditor(ref new ImageVM(std::make_shared<Image>(std::make_shared<GraphDevice>()), true))
{
}

anim::ImageEditor::~ImageEditor()
{
	this->Destroy();
}

void anim::ImageEditor::Destroy()
{
	if (this->image != nullptr)
	{
		this->image->PropertyChanged -= this->imageChangedToken;
		this->image = nullptr;
	}
}

anim::ImageVM ^anim::ImageEditor::State::get()
{
	return this->image;
}

void anim::ImageEditor::OnImagePropertyChanged(Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args)
{
}
