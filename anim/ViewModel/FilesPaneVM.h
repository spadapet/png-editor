#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;
	ref class ProjectFolderVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FilesPaneVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		FilesPaneVM(AppState *app);

	public:
		virtual ~FilesPaneVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^ProjectFolders
		{
			Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^get();
		}

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void AppPropertyChanged(const char *name);

		AppState *app;
		EventCookie appChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;
		Platform::Collections::Vector<ProjectFolderVM ^> ^projectFolders;
	};
}
