#include "pch.h"
#include "UI/Shell.xaml.h"

anim::Shell::Shell(AppState *app)
	: state(ref new ShellVM(app))
{
	this->InitializeComponent();
}

anim::Shell::~Shell()
{
}

anim::ShellVM ^anim::Shell::State::get()
{
	return this->state;
}
