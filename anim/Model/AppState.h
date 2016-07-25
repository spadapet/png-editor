#pragma once

#include "Core/Event.h"
#include "Model/AppPaneInfo.h"

namespace anim
{
	class AppState
	{
	public:
		AppState();
		~AppState();

		SimpleEvent Disposed;
		ChangedEvent PropertyChanged;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderAdded;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderRemoved;

		void Initialize();
		const std::vector<std::unique_ptr<AppPaneInfo>> &GetPanes() const;
		const std::vector<Windows::Storage::StorageFolder ^> &GetProjectFolders() const;
		AppPaneInfo &GetNonePane();

	private:
		AppPaneInfo nonePane;
		std::vector<std::unique_ptr<AppPaneInfo>> panes;
		std::vector<Windows::Storage::StorageFolder ^> projectFolders;
	};
}
