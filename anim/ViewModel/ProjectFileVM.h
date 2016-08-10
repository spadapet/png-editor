#pragma once

namespace anim
{
	class ProjectFile;

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectFileVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ProjectFileVM(std::shared_ptr<ProjectFile> file);
		ProjectFileVM(Windows::Storage::StorageFile ^file);

	public:
		ProjectFileVM();
		virtual ~ProjectFileVM();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Storage::StorageFile ^File { Windows::Storage::StorageFile ^get(); }
		property Platform::String ^DisplayName { Platform::String ^get(); }
		property Platform::String ^FullPath { Platform::String ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		std::shared_ptr<ProjectFile> file;
	};
}
