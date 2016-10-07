#pragma once

#include "Core/Event.h"

namespace anim
{
	class Image;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImageVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ImageVM(std::shared_ptr<Image> image);

	public:
		virtual ~ImageVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		std::shared_ptr<Image> image;
	};
}
