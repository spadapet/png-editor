#pragma once

namespace anim
{
	class ProjectFolder
	{
	public:
		ProjectFolder(Windows::Storage::StorageFolder ^folder);
		~ProjectFolder();

		Windows::Storage::StorageFolder ^GetFolder() const;

	private:
		Windows::Storage::StorageFolder ^folder;
	};
}
