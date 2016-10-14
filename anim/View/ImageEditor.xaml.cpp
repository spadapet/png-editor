#include "pch.h"
#include "Core/GraphDevice.h"
#include "Model/Image.h"
#include "View/ImageEditor.xaml.h"
#include "ViewModel/ImageVM.h"

anim::ImageEditor::ImageEditor(ImageVM ^image)
	: state(ref new ImageEditorVM(image))
{
	this->DataContext = this->state;
	this->InitializeComponent();
}

anim::ImageEditor::ImageEditor()
	: ImageEditor(ref new ImageVM())
{
}

anim::ImageEditor::~ImageEditor()
{
	this->Destroy();
}

void anim::ImageEditor::Destroy()
{
	this->state->Destroy();
}

anim::ImageEditorVM ^anim::ImageEditor::State::get()
{
	return this->state;
}

void anim::ImageEditor::OnImagePropertyChanged(Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args)
{
}
