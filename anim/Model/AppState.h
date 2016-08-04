#pragma once

#include "Core/Event.h"

namespace anim
{
	class PaneInfo;

	enum class AppMode
	{
		None,
		Edit,
	};

	class AppState : public std::enable_shared_from_this<AppState>
	{
	public:
		AppState();
		~AppState();

		static std::shared_ptr<AppState> CreateForDesigner();
		static void AssertDesigner();
		static bool IsDesigner();

		ChangedEvent PropertyChanged;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderAdded;
		Event<Windows::Storage::StorageFolder ^> ProjectFolderRemoved;

		void Initialize();
		void InitializeForDesigner();
		void Load();
		void Save();

		const std::vector<std::shared_ptr<PaneInfo>> &GetPanes() const;
		const std::vector<Windows::Storage::StorageFolder ^> &GetProjectFolders() const;
		std::shared_ptr<PaneInfo> GetNonePane() const;
		AppMode GetMode() const;

	private:
		std::shared_ptr<PaneInfo> nonePane;
		std::vector<std::shared_ptr<PaneInfo>> panes;
		std::vector<Windows::Storage::StorageFolder ^> projectFolders;
	};
}
