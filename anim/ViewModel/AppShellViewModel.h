#pragma once

#include "Core/Event.h"
#include "ViewModel/ProjectFolderViewModel.h"

namespace anim
{
	class AppState;
	ref class AppPaneInfoViewModel;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AppShellViewModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		AppShellViewModel();
		virtual ~AppShellViewModel();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<AppPaneInfoViewModel ^> ^Panes { Windows::Foundation::Collections::IVector<AppPaneInfoViewModel ^> ^get(); }
		property Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^ProjectFolders { Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^get(); }
		property AppPaneInfoViewModel ^ActivePane { AppPaneInfoViewModel ^get(); void set(AppPaneInfoViewModel ^value); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ModelPropertyChanged(const char *name);

		AppState *parent;
		EventCookie parentDisposedCookie;
		EventCookie parentChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;

		AppPaneInfoViewModel ^nonePane;
		AppPaneInfoViewModel ^activePane;
		Platform::Collections::Vector<AppPaneInfoViewModel ^> ^panes;
		Platform::Collections::Vector<ProjectFolderViewModel ^> ^projectFolders;
	};
}
