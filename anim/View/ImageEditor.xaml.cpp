#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/Image.h"
#include "View/ImageEditor.xaml.h"
#include "ViewModel/ImageVM.h"

anim::ImageEditor::ImageEditor(ImageVM ^image)
	: image(image)
{
	this->DataContext = image;
	this->InitializeComponent();
}

anim::ImageEditor::ImageEditor()
	: ImageEditor(ref new ImageVM(std::make_shared<Image>(std::make_shared<GraphDevice>())))
{
}

anim::ImageEditor::~ImageEditor()
{
}

anim::ImageVM ^anim::ImageEditor::State::get()
{
	return this->image;
}
