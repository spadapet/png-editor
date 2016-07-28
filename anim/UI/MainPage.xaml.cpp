#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/MainPageViewModel.h"

anim::MainPage::MainPage(AppState *app)
	: state(ref new MainPageViewModel())
	, shell(ref new AppShell(app))
{
	this->InitializeComponent();
}

anim::MainPage::~MainPage()
{
}

anim::MainPageViewModel ^anim::MainPage::State::get()
{
	return this->state;
}

Windows::UI::Xaml::UIElement ^anim::MainPage::Shell::get()
{
	return this->shell;
}
