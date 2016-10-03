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


void anim::ImageEditor::OnRegionsInvalidated(
	Microsoft::Graphics::Canvas::UI::Xaml::CanvasVirtualControl ^sender,
	Microsoft::Graphics::Canvas::UI::Xaml::CanvasRegionsInvalidatedEventArgs ^args)
{
	Windows::Foundation::Rect visibleRect = args->VisibleRegion;

	for (Windows::Foundation::Rect rect : args->InvalidatedRegions)
	{
		Microsoft::Graphics::Canvas::CanvasDrawingSession ^session = sender->CreateDrawingSession(rect);
	}
}
