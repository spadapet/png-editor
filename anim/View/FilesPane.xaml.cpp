#include "pch.h"
#include "Core/Command.h"
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

void anim::FilesPane::OnProjectItemContextMenuOpened(Platform::Object ^sender, Platform::Object ^args)
{
	Windows::UI::Xaml::Controls::MenuFlyout ^flyout = dynamic_cast<Windows::UI::Xaml::Controls::MenuFlyout ^>(sender);
	bool selected = false;

	for (Windows::UI::Xaml::Controls::MenuFlyoutItemBase ^itemBase : flyout->Items)
	{
		if (!selected)
		{
			selected = true;

			IProjectItemVM ^item = dynamic_cast<IProjectItemVM ^>(itemBase->DataContext);
			if (item != nullptr)
			{
				this->ProjectList->SelectSingle(item);
			}
		}

		Windows::UI::Xaml::Controls::MenuFlyoutItem ^flyoutItem = dynamic_cast<Windows::UI::Xaml::Controls::MenuFlyoutItem ^>(itemBase);
		Command ^command = dynamic_cast<anim::Command ^>(flyoutItem->Command);
		if (command != nullptr)
		{
			command->NotifyCanExecuteChanged();
		}
	}
}
