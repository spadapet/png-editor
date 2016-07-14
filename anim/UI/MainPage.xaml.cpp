#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/MainPageViewModel.h"

anim::MainPage::MainPage()
	: state(ref new MainPageViewModel())
{
	this->InitializeComponent();
	this->MainContent->Content = ref new AppShell();
}

anim::MainPage::~MainPage()
{
}

anim::MainPageViewModel ^anim::MainPage::State::get()
{
	return this->state;
}
