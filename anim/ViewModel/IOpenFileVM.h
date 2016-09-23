#pragma once

namespace anim
{
	ref class OpenImageVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public interface class IOpenFileVM : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
		void Destroy();

		property Windows::Storage::StorageFile ^File { Windows::Storage::StorageFile ^get(); }
		property Platform::String ^Name { Platform::String ^get(); }
		property Platform::String ^Tooltip { Platform::String ^get(); }
		property Platform::String ^Path { Platform::String ^get(); }
		property bool IsDirty { bool get(); }
		property bool IsActive { bool get(); void set(bool value); }
		property bool TabMouseHover { bool get(); void set(bool value); }
		property bool ShowCloseButton { bool get(); }
		property Windows::UI::Xaml::UIElement ^UserInterface { Windows::UI::Xaml::UIElement ^get(); }
		property Windows::UI::Xaml::Input::ICommand ^CloseCommand { Windows::UI::Xaml::Input::ICommand ^get(); }

		property OpenImageVM ^AsImage { OpenImageVM ^get(); }
	};
}
