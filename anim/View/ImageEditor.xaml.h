#pragma once

#include "View/ImageEditor.g.h"

namespace anim
{
	ref class OpenImageVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImageEditor sealed
	{
	internal:
		ImageEditor(OpenImageVM ^image);

	public:
		ImageEditor();
		virtual ~ImageEditor();

		property OpenImageVM ^State { OpenImageVM ^get(); }

	private:
		void OnRegionsInvalidated(Microsoft::Graphics::Canvas::UI::Xaml::CanvasVirtualControl ^sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasRegionsInvalidatedEventArgs ^args);
		void OnCreateResources(Microsoft::Graphics::Canvas::UI::Xaml::CanvasVirtualControl ^sender, Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs ^args);

		OpenImageVM ^image;
	};
}
