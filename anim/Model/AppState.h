#pragma once

#include "Core/Event.h"

namespace anim
{
	class PaneInfo;
	class ProjectFolder;

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

		static std::shared_ptr<AppState> Create();
		static std::shared_ptr<AppState> CreateForDesigner();

		// Events
		ChangedEvent PropertyChanged;
		Event<std::shared_ptr<ProjectFolder>> ProjectFolderAdded;
		Event<std::shared_ptr<ProjectFolder>> ProjectFolderRemoved;

		// Properties
		const std::vector<std::shared_ptr<PaneInfo>> &GetPanes() const;
		const std::vector<std::shared_ptr<anim::ProjectFolder>> &GetProjectFolders() const;
		std::shared_ptr<PaneInfo> GetNonePane() const;
		AppMode GetMode() const;

		// Methods
		void AddProjectFolder(Windows::Storage::StorageFolder ^folder);

	private:
		void Initialize();
		void InitializeForDesigner();
		void ResetPanes();

		std::shared_ptr<PaneInfo> nonePane;
		std::vector<std::shared_ptr<PaneInfo>> panes;
		std::vector<std::shared_ptr<ProjectFolder>> projectFolders;
	};
}
