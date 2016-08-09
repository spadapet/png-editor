#pragma once

#include "ViewModel/ProjectFileVM.h"

namespace anim
{
	class ProjectFolder;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectFolderVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ProjectFolderVM(std::shared_ptr<ProjectFolder> folder);

	public:
		ProjectFolderVM();
		virtual ~ProjectFolderVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Storage::StorageFolder ^Folder { Windows::Storage::StorageFolder ^get(); }
		property Platform::String ^DisplayName { Platform::String ^get(); }
		property Platform::String ^FullPath { Platform::String ^get(); }

		property Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^Folders
		{
			Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^get();
		}

		property Windows::Foundation::Collections::IVector<ProjectFileVM ^> ^Files
		{
			Windows::Foundation::Collections::IVector<ProjectFileVM ^> ^get();
		}

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		std::shared_ptr<ProjectFolder> folder;
		Platform::Collections::Vector<ProjectFolderVM ^> ^folders;
		Platform::Collections::Vector<ProjectFileVM ^> ^files;
	};
}
