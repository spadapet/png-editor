#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "ViewModel/MainPageViewModel.h"

anim::MainPageViewModel::MainPageViewModel(AppState *app)
	: shell(ref new AppShell(app))
{
}

anim::MainPageViewModel::~MainPageViewModel()
{
}

Windows::UI::Xaml::UIElement ^anim::MainPageViewModel::Shell::get()
{
	return this->shell;
}
