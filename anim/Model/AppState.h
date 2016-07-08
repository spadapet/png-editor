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

		const std::vector<Windows::Storage::StorageFolder ^> &GetProjectFolders() const;

	private:
		std::vector<Windows::Storage::StorageFolder ^> projectFolders;
	};
}
