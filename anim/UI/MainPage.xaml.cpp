#include "pch.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/MainPageViewModel.h"

anim::MainPage::MainPage()
	: state(ref new MainPageViewModel())
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
