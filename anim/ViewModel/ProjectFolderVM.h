#pragma once

#include "Core/Event.h"
#include "ViewModel/IProjectItemVM.h"

namespace anim
{
	class ProjectFolder;
	class ProjectItem;
	ref class FlatProjectItems;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectFolderVM sealed
		: Windows::UI::Xaml::Data::INotifyPropertyChanged
		, IProjectItemVM
	{
	internal:
		ProjectFolderVM(std::shared_ptr<ProjectFolder> folder, ProjectFolderVM ^parent);

	public:
		ProjectFolderVM();
		virtual ~ProjectFolderVM();

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		// IProjectItemVM
		virtual property Windows::Storage::IStorageItem ^Item { Windows::Storage::IStorageItem ^get(); }
		virtual property Platform::String ^DisplayName { Platform::String ^get(); }
		virtual property Platform::String ^FullPath { Platform::String ^get(); }
		virtual property int Level { int get(); }
		virtual property ProjectFileVM ^AsFile { ProjectFileVM ^get(); }
		virtual property ProjectFolderVM ^AsFolder { ProjectFolderVM ^get(); }
		virtual property IProjectItemVM ^Parent { IProjectItemVM ^get(); }
		virtual property Windows::UI::Xaml::Input::ICommand ^ActivateCommand { Windows::UI::Xaml::Input::ICommand ^get(); };

		property Windows::Storage::StorageFolder ^Folder { Windows::Storage::StorageFolder ^get(); }
		property Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^Items { Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^get(); }
		property Windows::UI::Xaml::Interop::IBindableObservableVector ^BindableItems { Windows::UI::Xaml::Interop::IBindableObservableVector ^get(); }
		property Windows::UI::Xaml::Interop::IBindableObservableVector ^BindableFlatItems { Windows::UI::Xaml::Interop::IBindableObservableVector ^get(); }
		property bool HasItems { bool get(); }
		property bool ShowExpanded { bool get(); void set(bool value); }
		property bool ShowExpandedIcon { bool get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void FolderPropertyChanged(const char *name);
		void UpdateItems();
		IProjectItemVM ^MakeVM(std::shared_ptr<ProjectItem> item);

		std::shared_ptr<ProjectFolder> folder;
		Platform::WeakReference weakParent;
		EventCookie folderChangedCookie;
		Platform::Collections::Vector<IProjectItemVM ^> ^items;
		FlatProjectItems ^flatItems;
		bool expanded;
	};
}
