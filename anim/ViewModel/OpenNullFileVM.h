#pragma once

#include "Core/Event.h"
#include "ViewModel/IOpenFileVM.h"

namespace anim
{
	ref class OpenImageVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenNullFileVM sealed : IOpenFileVM
	{
	public:
		OpenNullFileVM();
		virtual ~OpenNullFileVM();

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		// IOpenFileVM
		virtual void Destroy();
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

		bool active;
	};
}
