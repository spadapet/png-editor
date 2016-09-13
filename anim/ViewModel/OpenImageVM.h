#pragma once

#include "ViewModel/IOpenFileVM.h"

namespace anim
{
	class OpenImageFile;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenImageVM sealed
		: Windows::UI::Xaml::Data::INotifyPropertyChanged
		, IOpenFileVM
	{
	internal:
		OpenImageVM(std::shared_ptr<OpenImageFile> file);
		property std::shared_ptr<OpenImageFile> Model { std::shared_ptr<OpenImageFile> get(); }

	public:
		OpenImageVM();
		virtual ~OpenImageVM();

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		// IOpenFileVM
		virtual void Destroy();
		virtual property Platform::String ^Name { Platform::String ^get(); }
		virtual property Platform::String ^Tooltip { Platform::String ^get(); }
		virtual property bool IsDirty { bool get(); }
		virtual property Windows::UI::Xaml::UIElement ^UserInterface { Windows::UI::Xaml::UIElement ^get(); }
		virtual property Windows::UI::Xaml::Input::ICommand ^CloseCommand { Windows::UI::Xaml::Input::ICommand ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		std::shared_ptr<OpenImageFile> file;
	};
}
