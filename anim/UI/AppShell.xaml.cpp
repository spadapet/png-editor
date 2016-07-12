#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "ViewModel/AppShellState.h"

anim::AppShell::AppShell()
	: state(ref new AppShellState())
{
	this->InitializeComponent();
}

anim::AppShell::~AppShell()
{
}

anim::AppShellState ^anim::AppShell::State::get()
{
	return this->state;
}
