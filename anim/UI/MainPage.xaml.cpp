#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "UI/MainPage.xaml.h"

anim::MainPage::MainPage(AppState *app)
	: state(ref new MainPageViewModel(app))
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
