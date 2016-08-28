#pragma once

#include "ViewModel/IProjectItemVM.h"

namespace anim
{
	class ProjectItem;
	ref class ProjectFolderVM;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectItemVM sealed
		: Windows::UI::Xaml::Data::INotifyPropertyChanged
		, IProjectItemVM
	{
	internal:
		ProjectItemVM(std::shared_ptr<ProjectItem> file, ProjectFolderVM ^parent);

	public:
		ProjectItemVM();
		virtual ~ProjectItemVM();

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		// IProjectItemVM
		virtual property Windows::Storage::IStorageItem ^Item { Windows::Storage::IStorageItem ^get(); }
		virtual property Platform::String ^DisplayName { Platform::String ^get(); }
		virtual property Platform::String ^FullPath { Platform::String ^get(); }
		virtual property int Level { int get(); }
		virtual property ProjectFileVM ^AsFile { ProjectFileVM ^get(); }
		virtual property ProjectFolderVM ^AsFolder { ProjectFolderVM ^get(); }
		virtual property Windows::UI::Xaml::Input::ICommand ^ActivateCommand { Windows::UI::Xaml::Input::ICommand ^get(); };

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		std::shared_ptr<ProjectItem> item;
		Platform::WeakReference weakParent;
	};
}
