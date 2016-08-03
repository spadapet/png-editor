#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectFolderVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ProjectFolderVM(Windows::Storage::StorageFolder ^folder);

	public:
		ProjectFolderVM();
		virtual ~ProjectFolderVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Storage::StorageFolder ^Folder { Windows::Storage::StorageFolder ^get(); }
		property Platform::String ^DisplayName { Platform::String ^get(); }
		property Platform::String ^FullName { Platform::String ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		Windows::Storage::StorageFolder ^folder;
	};
}
