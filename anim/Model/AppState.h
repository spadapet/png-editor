#pragma once

#include "Core/Event.h"
#include "Model/PaneInfo.h"

namespace anim
{
	enum class AppMode
	{
		None,
		Edit,
	};

	class AppState
	{
	public:
		AppState();
		~AppState();

		SimpleEvent Destroyed;
		ChangedEvent PropertyChanged;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderAdded;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderRemoved;

		void Initialize();
		void Save();
		const std::vector<std::unique_ptr<PaneInfo>> &GetPanes() const;
		const std::vector<Windows::Storage::StorageFolder ^> &GetProjectFolders() const;
		PaneInfo &GetNonePane();
		AppMode GetMode() const;

	private:
		PaneInfo nonePane;
		std::vector<std::unique_ptr<PaneInfo>> panes;
		std::vector<Windows::Storage::StorageFolder ^> projectFolders;
	};
}
