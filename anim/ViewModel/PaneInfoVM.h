#pragma once

#include "Core/Event.h"

namespace anim
{
	class PaneInfo;
	class AppState;

	ref class ShellVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PaneInfoVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		PaneInfoVM(std::shared_ptr<AppState> app, std::shared_ptr<PaneInfo> pane, ShellVM ^shell);
		PaneInfoVM(std::shared_ptr<AppState> app);

	public:
		PaneInfoVM();
		virtual ~PaneInfoVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Platform::String ^Name { Platform::String ^get(); }
		property Windows::UI::Xaml::Media::ImageSource ^Icon { Windows::UI::Xaml::Media::ImageSource ^get(); }
		property Windows::UI::Xaml::UIElement ^Pane { Windows::UI::Xaml::UIElement ^get(); }
		property Windows::UI::Xaml::Input::ICommand ^ToggleActiveCommand { Windows::UI::Xaml::Input::ICommand ^get(); }
		property bool IsActive { bool get(); void set(bool value); }
		property bool IsVisible { bool get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void AppPropertyChanged(const char *name);
		void PanePropertyChanged(const char *name);
		void ToggleActive(ShellVM ^shell);

		std::shared_ptr<AppState> app;
		std::shared_ptr<PaneInfo> pane;
		EventCookie appChangedCookie;
		EventCookie paneChangedCookie;
		Platform::String ^name;
		Windows::UI::Xaml::Media::ImageSource ^icon;
		Windows::UI::Xaml::UIElement ^paneUi;
		Windows::UI::Xaml::Input::ICommand ^toggleActiveCommand;
		bool active;
	};
}
