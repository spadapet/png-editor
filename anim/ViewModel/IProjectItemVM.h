#pragma once

namespace anim
{
	ref class ProjectFileVM;
	ref class ProjectFolderVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public interface class IProjectItemVM
	{
		property Windows::Storage::IStorageItem ^Item { Windows::Storage::IStorageItem ^get(); }
		property Platform::String ^DisplayName { Platform::String ^get(); }
		property Platform::String ^FullPath { Platform::String ^get(); }
		property int Level { int get(); }

		property ProjectFileVM ^AsFile { ProjectFileVM ^get(); }
		property ProjectFolderVM ^AsFolder { ProjectFolderVM ^get(); }

		property Windows::UI::Xaml::Input::ICommand ^ActivateCommand { Windows::UI::Xaml::Input::ICommand ^get(); }
	};
}
