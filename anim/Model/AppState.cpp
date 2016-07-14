#include "pch.h"
#include "Model/AppPane.h"
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
	this->panes.emplace_back(std::make_unique<AppPane>(AppPaneType::Files,
		[](AppPaneType type) -> Windows::UI::Xaml::UIElement ^
		{
			return nullptr;
		}));
}

const std::vector<std::unique_ptr<anim::AppPane>> &anim::AppState::GetPanes() const
{
	return this->panes;
}

const std::vector<Windows::Storage::StorageFolder ^> &anim::AppState::GetProjectFolders() const
{
	return this->projectFolders;
}
