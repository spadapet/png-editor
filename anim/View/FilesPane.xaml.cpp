#include "pch.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "View/FilesPane.xaml.h"
#include "ViewModel/IProjectItemVM.h"

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


void anim::FilesPane::OnDataTemplateUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	anim::DisconnectDataTemplateBindings(sender);
}

void anim::FilesPane::OnProjectItemContextMenuOpened(Platform::Object ^sender, Platform::Object ^args)
{
	Windows::UI::Xaml::Controls::MenuFlyout ^flyout = dynamic_cast<Windows::UI::Xaml::Controls::MenuFlyout ^>(sender);
	if (flyout != nullptr && flyout->Items->Size > 0)
	{
		Platform::Object ^dc = flyout->Items->GetAt(0)->DataContext;
		IProjectItemVM ^item = dynamic_cast<IProjectItemVM ^>(dc);

		if (item != nullptr)
		{
			this->ProjectList->SelectSingle(item);
		}
	}
}
