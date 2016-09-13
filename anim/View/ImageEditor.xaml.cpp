#include "pch.h"
#include "View/ImageEditor.xaml.h"
#include "ViewModel/OpenImageVM.h"

anim::ImageEditor::ImageEditor(OpenImageVM ^image)
	: image(image)
{
	this->DataContext = image;
	this->InitializeComponent();
}

anim::ImageEditor::ImageEditor()
	: ImageEditor(ref new OpenImageVM())
{
}

anim::ImageEditor::~ImageEditor()
{
}

anim::OpenImageVM ^anim::ImageEditor::State::get()
{
	return this->image;
}
