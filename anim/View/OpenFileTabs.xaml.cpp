#include "pch.h"
#include "Controller/AppController.h"
#include "Core/Xaml.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "View/OpenFileTabs.xaml.h"
#include "ViewModel/OpenFileTabsVM.h"

anim::OpenFileTabs::OpenFileTabs(std::shared_ptr<AppState> app)
	: state(ref new OpenFileTabsVM(app))
{
	this->InitializeComponent();
	this->DropTabsList->SetPopup(this->DropTabsPopup);
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

void anim::OpenFileTabs::OnClickDropDown(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	this->DropTabsPopup->IsOpen = true;
}

void anim::OpenFileTabs::OnTabsPopupOpened(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	Windows::UI::Xaml::Controls::Primitives::Popup ^popup =
		safe_cast<Windows::UI::Xaml::Controls::Primitives::Popup ^>(sender);

	anim::PostToMainThread([popup]()
	{
		if (popup->IsOpen)
		{
			Windows::UI::Xaml::Controls::Control ^child = safe_cast<Windows::UI::Xaml::Controls::Control ^>(popup->Child);
			child->UpdateLayout();
			popup->HorizontalOffset = -child->ActualWidth + 28;
			child->Focus(Windows::UI::Xaml::FocusState::Programmatic);
		}
	});
}

void anim::OpenFileTabs::OnTabsPopupClosed(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	Windows::UI::Xaml::Controls::Primitives::Popup ^popup =
		safe_cast<Windows::UI::Xaml::Controls::Primitives::Popup ^>(sender);
	Windows::UI::Xaml::Controls::Button ^button = this->DropTabsButton;

	anim::PostToMainThread([popup, button]()
	{
		if (!popup->IsOpen)
		{
			button->Focus(Windows::UI::Xaml::FocusState::Programmatic);
		}
	});
}

void anim::OpenFileTabs::OnDropDownButtonKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args)
{
	if (!args->Handled && args->Key == Windows::System::VirtualKey::Down && !this->DropTabsPopup->IsOpen)
	{
		this->DropTabsPopup->IsOpen = true;
		args->Handled = true;
	}
}
