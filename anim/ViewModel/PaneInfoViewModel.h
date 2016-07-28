#pragma once

#include "Core/Event.h"
#include "ViewModel/ProjectFolderViewModel.h"

namespace anim
{
	class AppPaneInfo;
	class AppState;

	ref class ShellViewModel;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PaneInfoViewModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		PaneInfoViewModel(AppState *app, AppPaneInfo *pane, ShellViewModel ^shell);

	public:
		PaneInfoViewModel();
		virtual ~PaneInfoViewModel();

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
		void ModelPropertyChanged(const char *name);
		void ToggleActive(ShellViewModel ^shell);

		AppState *app;
		AppPaneInfo *pane;
		EventCookie appDisposedCookie;
		EventCookie appChangedCookie;
		EventCookie paneDisposedCookie;
		EventCookie paneChangedCookie;
		Platform::String ^name;
		Windows::UI::Xaml::Media::ImageSource ^icon;
		Windows::UI::Xaml::UIElement ^paneUi;
		Windows::UI::Xaml::Input::ICommand ^toggleActiveCommand;
		bool active;
	};
}
