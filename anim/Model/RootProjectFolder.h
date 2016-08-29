#pragma once

#include "Model/ProjectFolder.h"

namespace anim
{
	class AppState;

	class RootProjectFolder : public ProjectFolder
	{
	public:
		RootProjectFolder(Windows::Storage::StorageFolder ^folder, std::shared_ptr<AppState> app);
		virtual ~RootProjectFolder() override;

		virtual std::shared_ptr<AppState> GetAppState() const override;

	private:
		std::weak_ptr<AppState> app;
	};
}
