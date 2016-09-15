#include "pch.h"
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

static anim::IOpenFileVM ^GetFileFromSender(Platform::Object ^sender)
{
	Windows::UI::Xaml::DependencyObject ^elem = dynamic_cast<Windows::UI::Xaml::DependencyObject ^>(sender);
	if (elem != nullptr)
	{
		Platform::Object ^dc = elem->GetValue(Windows::UI::Xaml::Controls::Control::DataContextProperty);
		return dynamic_cast<anim::IOpenFileVM ^>(dc);
	}

	return nullptr;
}

void anim::OpenFileTabs::TabItem_PointerEntered(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	IOpenFileVM ^file = ::GetFileFromSender(sender);
	assert(file != nullptr);

	if (file != nullptr)
	{
		file->TabMouseHover = true;
	}
}

void anim::OpenFileTabs::TabItem_PointerExited(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	IOpenFileVM ^file = ::GetFileFromSender(sender);
	assert(file != nullptr);

	if (file != nullptr)
	{
		file->TabMouseHover = false;
	}
}
