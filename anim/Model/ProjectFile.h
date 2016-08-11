#pragma once

#include "Model/ProjectItem.h"

namespace anim
{
	class ProjectFile : public ProjectItem
	{
	public:
		ProjectFile(Windows::Storage::StorageFile ^file, std::shared_ptr<ProjectFolder> parent);
		virtual ~ProjectFile() override;

		Windows::Storage::StorageFile ^GetFile() const;
		void SetFile(Windows::Storage::StorageFile ^file);

	private:
		Windows::Storage::StorageFile ^file;
	};
}
