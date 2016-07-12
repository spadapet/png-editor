#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/MainPageState.h"

anim::MainPage::MainPage()
	: state(ref new MainPageState())
{
	this->InitializeComponent();
	this->MainContent->Content = ref new AppShell();
}

anim::MainPage::~MainPage()
{
}

anim::MainPageState ^anim::MainPage::State::get()
{
	return this->state;
}
