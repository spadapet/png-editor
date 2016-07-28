#include "pch.h"
#include "Model/AppPaneInfo.h"
#include "Model/AppState.h"
#include "UI/FilesPane.xaml.h"

static Windows::UI::Xaml::UIElement ^CreateNonePane(anim::AppPaneType type) 
{
	return nullptr;
}

anim::AppState::AppState()
	: nonePane(AppPaneType::None, &CreateNonePane)
{
}

anim::AppState::~AppState()
{
	Disposed.Notify();
}

void anim::AppState::Initialize()
{
	this->panes.emplace_back(std::make_unique<AppPaneInfo>(AppPaneType::Files,
		[this](AppPaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this);
	}));

	this->panes.emplace_back(std::make_unique<AppPaneInfo>(AppPaneType::Color,
		[this](AppPaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this);
	}));

	this->panes.emplace_back(std::make_unique<AppPaneInfo>(AppPaneType::Layers,
		[this](AppPaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this);
	}));

	this->panes.emplace_back(std::make_unique<AppPaneInfo>(AppPaneType::View,
		[this](AppPaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this);
	}));

	this->panes.emplace_back(std::make_unique<AppPaneInfo>(AppPaneType::Animation,
		[this](AppPaneType type) -> Windows::UI::Xaml::UIElement ^
	{
		return ref new FilesPane(this);
	}));
}

void anim::AppState::Save()
{
}

const std::vector<std::unique_ptr<anim::AppPaneInfo>> &anim::AppState::GetPanes() const
{
	return this->panes;
}

const std::vector<Windows::Storage::StorageFolder ^> &anim::AppState::GetProjectFolders() const
{
	return this->projectFolders;
}

anim::AppPaneInfo &anim::AppState::GetNonePane()
{
	return this->nonePane;
}

anim::AppMode anim::AppState::GetMode() const
{
	return AppMode::None;
}
