#include "pch.h"
#include "Model/AppState.h"
#include "UI/FilesPane.xaml.h"

anim::FilesPane::FilesPane(std::shared_ptr<AppState> app)
	: state(ref new FilesPaneVM(app))
{
	this->DataContext = this->state;
	this->InitializeComponent();
}

anim::FilesPane::FilesPane()
	: FilesPane(AppState::CreateForDesigner())
{
}

anim::FilesPane::~FilesPane()
{
}

anim::FilesPaneVM ^anim::FilesPane::State::get()
{
	return this->state;
}

void anim::FilesPane::OnClickAddFolderLink(Windows::UI::Xaml::Documents::Hyperlink ^sender, Windows::UI::Xaml::Documents::HyperlinkClickEventArgs ^args)
{
	this->state->AddFolderCommand->Execute(nullptr);
}
