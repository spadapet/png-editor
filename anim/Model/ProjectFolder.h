#pragma once

#include "Model/ProjectItem.h"

namespace anim
{
	class ProjectFolder : public ProjectItem
	{
	public:
		ProjectFolder(Windows::Storage::StorageFolder ^folder, std::shared_ptr<ProjectFolder> parent);
		virtual ~ProjectFolder() override;

		Windows::Storage::StorageFolder ^GetFolder() const;

	private:
		Windows::Storage::StorageFolder ^folder;
	};
}
