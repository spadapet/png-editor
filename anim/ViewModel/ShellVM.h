#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;
	ref class PaneInfoVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ShellVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ShellVM(AppState *app);

	public:
		virtual ~ShellVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<PaneInfoVM ^> ^Panes { Windows::Foundation::Collections::IVector<PaneInfoVM ^> ^get(); }
		property PaneInfoVM ^ActivePane { PaneInfoVM ^get(); void set(PaneInfoVM ^value); }
		property bool HasActivePane { bool get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void AppPropertyChanged(const char *name);

		// App
		AppState *app;
		EventCookie appChangedCookie;

		// Panes
		PaneInfoVM ^nonePane;
		PaneInfoVM ^activePane;
		Platform::Collections::Vector<PaneInfoVM ^> ^panes;
	};
}
