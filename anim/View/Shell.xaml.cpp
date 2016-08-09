﻿#include "pch.h"
#include "Model/AppState.h"
#include "View/Shell.xaml.h"

anim::Shell::Shell(std::shared_ptr<AppState> app)
	: state(ref new ShellVM(app))
{
	this->DataContext = this->state;
	this->InitializeComponent();
}

anim::Shell::Shell()
	: Shell(AppState::CreateForDesigner())
{
}

anim::Shell::~Shell()
{
}

anim::ShellVM ^anim::Shell::State::get()
{
	return this->state;
}