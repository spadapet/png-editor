#include "pch.h"
#include "Model/PaneInfo.h"
#include "Model/AppState.h"
#include "UI/FilesPane.xaml.h"

static Windows::UI::Xaml::UIElement ^CreateNonePane(anim::PaneType type) 
{
	return nullptr;
}

anim::AppState::AppState()
	: AppState(false)
{
}

anim::AppState::AppState(bool forDesigner)
	: nonePane(PaneType::None, &CreateNonePane)
{
	this->panes.emplace_back(std::make_unique<PaneInfo>(PaneType::Files,
		[this](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this->shared_from_this());
	}));

	this->panes.emplace_back(std::make_unique<PaneInfo>(PaneType::Color,
		[this](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this->shared_from_this());
	}));

	this->panes.emplace_back(std::make_unique<PaneInfo>(PaneType::Layers,
		[this](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this->shared_from_this());
	}));

	this->panes.emplace_back(std::make_unique<PaneInfo>(PaneType::View,
		[this](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this->shared_from_this());
	}));

	this->panes.emplace_back(std::make_unique<PaneInfo>(PaneType::Animation,
		[this](PaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this->shared_from_this());
	}));
}

anim::AppState::~AppState()
{
}

std::shared_ptr<anim::AppState> anim::AppState::CreateForDesigner()
{
	return std::make_shared<AppState>(true);
}

void anim::AppState::Load()
{
}

void anim::AppState::Save()
{
}

const std::vector<std::unique_ptr<anim::PaneInfo>> &anim::AppState::GetPanes() const
{
	return this->panes;
}

const std::vector<Windows::Storage::StorageFolder ^> &anim::AppState::GetProjectFolders() const
{
	return this->projectFolders;
}

anim::PaneInfo &anim::AppState::GetNonePane()
{
	return this->nonePane;
}

anim::AppMode anim::AppState::GetMode() const
{
	return AppMode::None;
}
