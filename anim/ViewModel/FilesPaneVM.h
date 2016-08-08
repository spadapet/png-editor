#pragma once

#include "Core/Event.h"
#include "Model/AppController.h"

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

		// Commands
		property Windows::UI::Xaml::Input::ICommand ^AddFolderCommand { Windows::UI::Xaml::Input::ICommand ^get(); }
		property Windows::UI::Xaml::Input::ICommand ^OpenFileCommand { Windows::UI::Xaml::Input::ICommand ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void AppPropertyChanged(const char *name);
		void ResetProjectFolders();

		std::shared_ptr<AppState> app;
		EventCookie appChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;
		Platform::Collections::Vector<ProjectFolderVM ^> ^projectFolders;

		// Commands
		AppController controller;
		Windows::UI::Xaml::Input::ICommand ^addFolderCommand;
		Windows::UI::Xaml::Input::ICommand ^openFileCommand;
	};
}
