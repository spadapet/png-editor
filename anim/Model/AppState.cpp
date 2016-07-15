#include "pch.h"
#include "Model/AppPaneInfo.h"
#include "Model/AppState.h"

anim::AppState::AppState()
{
}

anim::AppState::~AppState()
{
	Disposed.Notify();
}

void anim::AppState::Initialize()
{
	this->panes.emplace_back(std::make_unique<AppPaneInfo>(AppPaneType::Files,
		[](AppPaneType type) -> Windows::UI::Xaml::UIElement ^
		{
			return nullptr;
		}));
}

const std::vector<std::unique_ptr<anim::AppPaneInfo>> &anim::AppState::GetPanes() const
{
	return this->panes;
}

const std::vector<Windows::Storage::StorageFolder ^> &anim::AppState::GetProjectFolders() const
{
	return this->projectFolders;
}
