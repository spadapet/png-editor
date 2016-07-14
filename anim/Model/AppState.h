#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppPane;

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
		const std::vector<std::unique_ptr<AppPane>> &GetPanes() const;
		const std::vector<Windows::Storage::StorageFolder ^> &GetProjectFolders() const;

	private:
		std::vector<std::unique_ptr<AppPane>> panes;
		std::vector<Windows::Storage::StorageFolder ^> projectFolders;
	};
}
