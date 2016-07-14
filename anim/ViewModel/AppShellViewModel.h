#pragma once

#include "Core/Event.h"
#include "ViewModel/ProjectFolderViewModel.h"

namespace anim
{
	class AppState;
	ref class AppPaneViewModel;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AppShellViewModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		AppShellViewModel();
		virtual ~AppShellViewModel();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<AppPaneViewModel ^> ^Panes { Windows::Foundation::Collections::IVector<AppPaneViewModel ^> ^get(); }
		property Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^ProjectFolders { Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ModelPropertyChanged(const char *name);

		AppState *parent;
		EventCookie parentDisposedCookie;
		EventCookie parentChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;

		Platform::Collections::Vector<AppPaneViewModel ^> ^panes;
		Platform::Collections::Vector<ProjectFolderViewModel ^> ^projectFolders;
	};
}
