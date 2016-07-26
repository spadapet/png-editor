#include "pch.h"
#include "UI/AppShell.xaml.h"
#include "ViewModel/AppShellViewModel.h"

anim::AppShell::AppShell()
	: state(ref new AppShellViewModel())
{
	this->InitializeComponent();
}

anim::AppShell::~AppShell()
{
}

anim::AppShellViewModel ^anim::AppShell::State::get()
{
	return this->state;
}

void anim::AppShell::OnPaneButtonClick(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	Windows::UI::Xaml::Controls::Button ^button = dynamic_cast<Windows::UI::Xaml::Controls::Button ^>(sender);
	if (button != nullptr)
	{
		AppPaneInfoViewModel ^viewModel = dynamic_cast<AppPaneInfoViewModel ^>(button->DataContext);
		if (viewModel != nullptr)
		{
			this->State->ActivePane = (this->State->ActivePane != viewModel)
				? viewModel
				: nullptr;
		}
	}
}
