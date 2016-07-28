#pragma once

#include "Core/Event.h"
#include "ViewModel/ProjectFolderViewModel.h"

namespace anim
{
	class AppState;
	ref class PaneInfoViewModel;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ShellViewModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ShellViewModel(AppState *app);

	public:
		virtual ~ShellViewModel();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<PaneInfoViewModel ^> ^Panes { Windows::Foundation::Collections::IVector<PaneInfoViewModel ^> ^get(); }
		property Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^ProjectFolders { Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^get(); }
		property PaneInfoViewModel ^ActivePane { PaneInfoViewModel ^get(); void set(PaneInfoViewModel ^value); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ModelPropertyChanged(const char *name);

		AppState *app;
		EventCookie parentDisposedCookie;
		EventCookie parentChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;

		PaneInfoViewModel ^nonePane;
		PaneInfoViewModel ^activePane;
		Platform::Collections::Vector<PaneInfoViewModel ^> ^panes;
		Platform::Collections::Vector<ProjectFolderViewModel ^> ^projectFolders;
	};
}
