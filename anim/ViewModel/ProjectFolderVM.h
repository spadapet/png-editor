#pragma once

#include "Core/Event.h"
#include "ViewModel/IProjectItemVM.h"

namespace anim
{
	class ProjectFolder;
	class ProjectItem;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectFolderVM sealed
		: Windows::UI::Xaml::Data::INotifyPropertyChanged
		, IProjectItemVM
	{
	internal:
		ProjectFolderVM(std::shared_ptr<ProjectFolder> folder);

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

		property Windows::Storage::StorageFolder ^Folder { Windows::Storage::StorageFolder ^get(); }
		property Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^Items { Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^get(); }
		property bool HasItems { bool get(); }
		property bool ShowExpanded { bool get(); void set(bool value); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void FolderPropertyChanged(const char *name);
		void UpdateItems();
		IProjectItemVM ^MakeVM(std::shared_ptr<ProjectItem> item);

		std::shared_ptr<ProjectFolder> folder;
		EventCookie folderChangedCookie;
		Platform::Collections::Vector<IProjectItemVM ^> ^items;
		bool expanded;
	};
}
