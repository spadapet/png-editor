#include "pch.h"
#include "Model/AppState.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/MainPageVM.h"

anim::MainPage::MainPage(std::shared_ptr<AppState> app)
	: state(ref new MainPageVM(app))
{
	this->DataContext = this->state;
	this->InitializeComponent();
}

anim::MainPage::MainPage()
	: MainPage(AppState::CreateForDesigner())
{
	throw ref new Platform::NotImplementedException();
}

anim::MainPage::~MainPage()
{
}

anim::MainPageVM ^anim::MainPage::State::get()
{
	return this->state;
}
