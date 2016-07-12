#pragma once

#include "Core/Event.h"
#include "ViewModel/ProjectFolderViewModel.h"

namespace anim
{
	class AppState;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AppShellState sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		AppShellState();
		virtual ~AppShellState();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^ProjectFolders { Windows::Foundation::Collections::IVector<ProjectFolderViewModel ^> ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ModelPropertyChanged(const char *name);

		AppState *parent;
		EventCookie parentDisposedCookie;
		EventCookie parentChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;

		Platform::Collections::Vector<ProjectFolderViewModel ^> ^projectFolders;
	};
}
