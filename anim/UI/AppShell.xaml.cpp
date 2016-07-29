#include "pch.h"
#include "App.xaml.h"
#include "UI/AppShell.xaml.h"

anim::AppShell::AppShell(AppState *app)
	: state(ref new ShellVM(app))
{
	this->InitializeComponent();
}

anim::AppShell::~AppShell()
{
}

anim::ShellVM ^anim::AppShell::State::get()
{
	return this->state;
}
