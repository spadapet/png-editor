#include "pch.h"
#include "UI/AppShell.xaml.h"

anim::AppShell::AppShell()
	: state(ref new ShellViewModel())
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
