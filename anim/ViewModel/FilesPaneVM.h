#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;
	ref class ProjectFolderVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FilesPaneVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		FilesPaneVM(std::shared_ptr<AppState> app);

	public:
		FilesPaneVM();
		virtual ~FilesPaneVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^ProjectFolders
		{
			Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^get();
		}
		property bool HasFolders { bool get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void AppPropertyChanged(const char *name);

		std::shared_ptr<AppState> app;
		EventCookie appChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;
		Platform::Collections::Vector<ProjectFolderVM ^> ^projectFolders;
	};
}
