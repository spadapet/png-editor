#include "pch.h"
#include "UI/FilesPane.xaml.h"

anim::FilesPane::FilesPane(AppState *app)
	: state(ref new FilesPaneVM(app))
{
	this->InitializeComponent();
}

anim::FilesPane::~FilesPane()
{
}

anim::FilesPaneVM ^anim::FilesPane::State::get()
{
	return this->state;
}
