#pragma once

namespace anim
{
	class ProjectFile
	{
	public:
		ProjectFile(Windows::Storage::StorageFile ^file);
		~ProjectFile();

		Windows::Storage::StorageFile ^GetFile() const;

	private:
		Windows::Storage::StorageFile ^file;
	};
}
