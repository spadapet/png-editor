#include "pch.h"
#include "Controller/AppController.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "View/OpenFileTabs.xaml.h"
#include "ViewModel/OpenFileTabsVM.h"

anim::OpenFileTabs::OpenFileTabs(std::shared_ptr<AppState> app)
	: state(ref new OpenFileTabsVM(app))
{
	this->InitializeComponent();
	this->state->TabsList = this->TabsList;
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

void anim::OpenFileTabs::OnDataTemplateUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	anim::DisconnectDataTemplateBindings(sender);
}

void anim::OpenFileTabs::OnTabPointerEntered(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	IOpenFileVM ^file = anim::GetDataFromSender<IOpenFileVM>(sender);
	if (file != nullptr)
	{
		file->TabMouseHover = true;
	}
}

void anim::OpenFileTabs::OnTabPointerExited(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	IOpenFileVM ^file = anim::GetDataFromSender<IOpenFileVM>(sender);
	if (file != nullptr)
	{
		file->TabMouseHover = false;
	}
}

void anim::OpenFileTabs::OnClickOpenFile(Windows::UI::Xaml::Documents::Hyperlink ^sender, Windows::UI::Xaml::Documents::HyperlinkClickEventArgs ^args)
{
	anim::AppController controller(this->state->GetApp());
	controller.OpenFile();
}

void anim::OpenFileTabs::OnClickOpenFolder(Windows::UI::Xaml::Documents::Hyperlink ^sender, Windows::UI::Xaml::Documents::HyperlinkClickEventArgs ^args)
{
	anim::AppController controller(this->state->GetApp());
	controller.AddProjectFolder();
}
