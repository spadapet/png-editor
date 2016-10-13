#pragma once

#include "View/ImageEditor.g.h"
#include "ViewModel/ILayerVM.h"
#include "ViewModel/ImageVM.h"
#include "ViewModel/RasterLayerVM.h"

namespace anim
{
	ref class OpenImageVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImageEditor sealed
	{
	internal:
		ImageEditor(ImageVM ^image);

	public:
		ImageEditor();
		virtual ~ImageEditor();

		property ImageVM ^State { ImageVM ^get(); }

	private:
		ImageVM ^image;
	};
}
