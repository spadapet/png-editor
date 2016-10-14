#pragma once

#include "Core/Event.h"

namespace anim
{
	ref class ImageVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImageEditorVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ImageEditorVM(ImageVM ^image);

	public:
		ImageEditorVM();
		virtual ~ImageEditorVM();

		void Destroy();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property ImageVM ^Image { ImageVM ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		ImageVM ^image;
	};
}
