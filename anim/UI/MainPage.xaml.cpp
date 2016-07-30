#include "pch.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/MainPageVM.h"

anim::MainPage::MainPage(AppState *app)
	: state(ref new MainPageVM(app))
{
	this->InitializeComponent();
}

anim::MainPage::~MainPage()
{
}

anim::MainPageVM ^anim::MainPage::State::get()
{
	return this->state;
}
