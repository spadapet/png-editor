#pragma once

#include "Core/Event.h"
#include "ViewModel/IOpenFileVM.h"

namespace anim
{
	class OpenImageFile;
	ref class ImageEditor;
	ref class ImageVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenImageVM sealed : IOpenFileVM
	{
	internal:
		OpenImageVM(std::shared_ptr<OpenImageFile> file);
		property std::shared_ptr<OpenImageFile> Model { std::shared_ptr<OpenImageFile> get(); }

	public:
		OpenImageVM();
		virtual ~OpenImageVM();

		property ImageVM ^Image { ImageVM ^get(); }

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		// IOpenFileVM
		virtual void Destroy();
		virtual property Windows::Storage::StorageFile ^File { Windows::Storage::StorageFile ^get(); }
		virtual property Platform::String ^Name { Platform::String ^get(); }
		virtual property Platform::String ^Tooltip { Platform::String ^get(); }
		virtual property Platform::String ^Path { Platform::String ^get(); }
		virtual property bool IsDirty { bool get(); }
		virtual property bool IsActive { bool get(); void set(bool value); }
		virtual property bool TabMouseHover { bool get(); void set(bool value); }
		virtual property bool ShowCloseButton { bool get(); }
		virtual property Windows::UI::Xaml::UIElement ^UserInterface { Windows::UI::Xaml::UIElement ^get(); }
		virtual property Windows::UI::Xaml::Input::ICommand ^CloseCommand { Windows::UI::Xaml::Input::ICommand ^get(); }
		virtual property OpenImageVM ^AsImage { OpenImageVM ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void FilePropertyChanged(const char *name);
		void ResetImage();

		std::shared_ptr<OpenImageFile> file;
		EventCookie fileChangedCookie;
		ImageEditor ^imageEditor;
		ImageVM ^image;
		bool active;
		bool hover;
	};
}
