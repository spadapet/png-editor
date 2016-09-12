#pragma once

#include "Core/Event.h"
#include "ViewModel/IProjectItemVM.h"

namespace anim
{
	class ProjectFile;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectFileVM sealed
		: Windows::UI::Xaml::Data::INotifyPropertyChanged
		, IProjectItemVM
	{
	internal:
		ProjectFileVM(std::shared_ptr<ProjectFile> file, ProjectFolderVM ^parent);
		property std::shared_ptr<ProjectFile> Model { std::shared_ptr<ProjectFile> get(); }

	public:
		ProjectFileVM();
		virtual ~ProjectFileVM();

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		// IProjectItemVM
		virtual void Destroy();
		virtual property Windows::Storage::IStorageItem ^Item { Windows::Storage::IStorageItem ^get(); }
		virtual property Platform::String ^DisplayName { Platform::String ^get(); }
		virtual property Platform::String ^FullPath { Platform::String ^get(); }
		virtual property int Level { int get(); }
		virtual property ProjectFileVM ^AsFile { ProjectFileVM ^get(); }
		virtual property ProjectFolderVM ^AsFolder { ProjectFolderVM ^get(); }
		virtual property IProjectItemVM ^Parent { IProjectItemVM ^get(); }
		virtual property Windows::UI::Xaml::Input::ICommand ^ActivateCommand { Windows::UI::Xaml::Input::ICommand ^get(); }
		virtual property Windows::UI::Xaml::Input::ICommand ^DeleteCommand { Windows::UI::Xaml::Input::ICommand ^get(); }

		property Windows::Storage::StorageFile ^File { Windows::Storage::StorageFile ^get(); }
		property bool IsOpen { bool get(); }
		property Windows::UI::Xaml::Input::ICommand ^CloseCommand { Windows::UI::Xaml::Input::ICommand ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void FilePropertyChanged(const char *name);

		std::shared_ptr<ProjectFile> file;
		Platform::WeakReference weakParent;
		EventCookie fileChangedCookie;
	};
}
