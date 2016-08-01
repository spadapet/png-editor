#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MainPageVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		MainPageVM(AppState *app);

	public:
		virtual ~MainPageVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::UI::Xaml::UIElement ^UserInterface { Windows::UI::Xaml::UIElement ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void AppPropertyChanged(const char *name);

		AppState *app;
		EventCookie appChangedCookie;
		Windows::UI::Xaml::UIElement ^shell;
	};
}
