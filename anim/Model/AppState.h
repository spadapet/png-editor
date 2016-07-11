#pragma once

#include "Core/Event.h"

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

		const std::vector<Windows::Storage::StorageFolder ^> &GetProjectFolders() const;

	private:
		std::vector<Windows::Storage::StorageFolder ^> projectFolders;
	};
}
