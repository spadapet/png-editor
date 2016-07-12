#pragma once

namespace anim
{
	public ref class ProjectFolderViewModel sealed
	{
	public:
		ProjectFolderViewModel(Windows::Storage::StorageFolder ^folder);
		virtual ~ProjectFolderViewModel();
		void Dispose();

		property Windows::Storage::StorageFolder ^Folder { Windows::Storage::StorageFolder ^get(); }
		property Platform::String ^DisplayName { Platform::String ^get(); }
		property Platform::String ^FullName { Platform::String ^get(); }

	private:
		Windows::Storage::StorageFolder ^folder;
	};
}
