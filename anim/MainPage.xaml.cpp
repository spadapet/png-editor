#include "pch.h"
#include "MainPage.xaml.h"

anim::MainPage::MainPage(ViewAppState ^state)
	: state(state)
{
	this->InitializeComponent();
}

anim::MainPage::MainPage()
	: MainPage(nullptr)
{
}

anim::MainPage::~MainPage()
{
}

anim::ViewAppState ^anim::MainPage::State::get()
{
	return this->state;
}
