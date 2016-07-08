#include "pch.h"
#include "Model/AppState.h"

anim::AppState::AppState()
{
}

anim::AppState::~AppState()
{
	Disposed.Notify();
}

const std::vector<Windows::Storage::StorageFolder ^> &anim::AppState::GetProjectFolders() const
{
	return this->projectFolders;
}
