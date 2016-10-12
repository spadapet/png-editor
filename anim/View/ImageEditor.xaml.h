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
		ImageEditor(OpenImageVM ^image);

	public:
		ImageEditor();
		virtual ~ImageEditor();

		property OpenImageVM ^State { OpenImageVM ^get(); }

	private:
		OpenImageVM ^image;
	};
}
