#pragma once

namespace anim
{
	public ref class ProjectFolderVM sealed
	{
	internal:
		ProjectFolderVM(Windows::Storage::StorageFolder ^folder);

	public:
		virtual ~ProjectFolderVM();

		property Windows::Storage::StorageFolder ^Folder { Windows::Storage::StorageFolder ^get(); }
		property Platform::String ^DisplayName { Platform::String ^get(); }
		property Platform::String ^FullName { Platform::String ^get(); }

	private:
		Windows::Storage::StorageFolder ^folder;
	};
}
