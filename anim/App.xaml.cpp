#include "pch.h"
#include "App.xaml.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/ShellVM.h"

anim::App::App()
{
	this->InitializeComponent();

	this->Suspending += ref new Windows::UI::Xaml::SuspendingEventHandler(this, &App::OnSuspending);
	this->Resuming += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &App::OnResuming);
}

void anim::App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs ^args)
{
	this->InitializeProcess();

	switch (args->PreviousExecutionState)
	{
	case Windows::ApplicationModel::Activation::ApplicationExecutionState::Running:
	case Windows::ApplicationModel::Activation::ApplicationExecutionState::Suspended:
		// great, keep on running
		break;

	default:
	case Windows::ApplicationModel::Activation::ApplicationExecutionState::NotRunning:
	case Windows::ApplicationModel::Activation::ApplicationExecutionState::ClosedByUser:
	case Windows::ApplicationModel::Activation::ApplicationExecutionState::Terminated:
		{
			auto initGlobals = concurrency::task<void>([this]()
			{
				this->InitializeGlobals();
			});

			initGlobals.then([this]()
			{
				this->InitializeWindow(Windows::UI::Xaml::Window::Current);
			}, concurrency::task_continuation_context::use_current());
		}
		break;
	}
}

void anim::App::InitializeProcess()
{
}

void anim::App::InitializeGlobals()
{
	this->state.Initialize();
}

void anim::App::InitializeWindow(Windows::UI::Xaml::Window ^window)
{
	window->Content = ref new MainPage(&this->state);
	window->Activate();
}

void anim::App::OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args)
{
	Windows::ApplicationModel::SuspendingDeferral ^deferral = args->SuspendingOperation->GetDeferral();

	Windows::UI::Xaml::Window::Current->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler([this, deferral]()
	{
		this->state.Save();
		deferral->Complete();
	}));
}

void anim::App::OnResuming(Platform::Object ^sender, Platform::Object ^arg)
{
}
