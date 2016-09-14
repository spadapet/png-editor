#include "pch.h"
#include "Model/AppState.h"
#include "View/OpenFileTabs.xaml.h"
#include "ViewModel/OpenFileTabsVM.h"

anim::OpenFileTabs::OpenFileTabs(std::shared_ptr<AppState> app)
	: state(ref new OpenFileTabsVM(app))
{
	this->InitializeComponent();
}

anim::OpenFileTabs::OpenFileTabs()
	: OpenFileTabs(AppState::CreateForDesigner())
{
}

anim::OpenFileTabs::~OpenFileTabs()
{
}

anim::OpenFileTabsVM ^anim::OpenFileTabs::State::get()
{
	return this->state;
}
