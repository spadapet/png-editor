#include "pch.h"
#include "Model/AppState.h"
#include "View/FilesPane.xaml.h"

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
	FrameworkElement ^elem = dynamic_cast<Windows::UI::Xaml::FrameworkElement ^>(sender);
	if (elem != nullptr)
	{
		Windows::UI::Xaml::IDataTemplateExtension ^data = Windows::UI::Xaml::DataTemplate::GetExtensionInstance(elem);
		if (data != nullptr)
		{
			data->ResetTemplate();
		}
	}
}
