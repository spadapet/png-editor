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

	class AppState : public std::enable_shared_from_this<AppState>
	{
	public:
		AppState();
		AppState(bool forDesigner);
		~AppState();

		static std::shared_ptr<AppState> CreateForDesigner();
		static void AssertDesigner();
		static bool IsDesigner();

		ChangedEvent PropertyChanged;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderAdded;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderRemoved;

		void Load();
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
