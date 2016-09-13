#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;
	ref class PaneInfoVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ShellVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ShellVM(std::shared_ptr<AppState> app);

	public:
		ShellVM();
		virtual ~ShellVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<PaneInfoVM ^> ^Panes { Windows::Foundation::Collections::IVector<PaneInfoVM ^> ^get(); }
		property PaneInfoVM ^ActivePane { PaneInfoVM ^get(); void set(PaneInfoVM ^value); }
		property bool HasActivePane { bool get(); }
		property double PaneWidth { double get(); void set(double value); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void AppPropertyChanged(const char *name);
		void PanePropertyChanged(Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args);
		void ResetPanes();

		// App
		std::shared_ptr<AppState> app;
		EventCookie appChangedCookie;

		// Panes
		PaneInfoVM ^nonePane;
		PaneInfoVM ^activePane;
		Platform::Collections::Vector<PaneInfoVM ^> ^panes;
		Windows::Foundation::EventRegistrationToken paneChangedCookie;
		double paneWidth;
	};
}
