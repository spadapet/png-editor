#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "ViewModel/AppShellViewModel.h"

anim::AppShell::AppShell()
	: state(ref new AppShellViewModel())
{
	this->InitializeComponent();
}

anim::AppShell::~AppShell()
{
}

anim::AppShellViewModel ^anim::AppShell::State::get()
{
	return this->state;
}
