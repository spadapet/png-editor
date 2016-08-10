#pragma once

#include "Model/ProjectFolder.h"

namespace anim
{
	class RootProjectFolder : public ProjectFolder
	{
	public:
		RootProjectFolder(Windows::Storage::StorageFolder ^folder);
		~RootProjectFolder();

		virtual bool IsRoot() const override;
	};
}
