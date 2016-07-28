#include "pch.h"
#include "App.xaml.h"
#include "UI/AppShell.xaml.h"

anim::AppShell::AppShell(AppState *app)
	: state(ref new ShellViewModel(app))
{
	this->InitializeComponent();
}

anim::AppShell::~AppShell()
{
}

anim::ShellViewModel ^anim::AppShell::State::get()
{
	return this->state;
}
