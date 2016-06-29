#include "pch.h"
#include "App.xaml.h"
#include "MainPage.xaml.h"
#include "ViewModel/ViewAppState.h"

anim::App::App()
{
	this->InitializeComponent();

	this->Suspending += ref new Windows::UI::Xaml::SuspendingEventHandler(this, &App::OnSuspending);
	this->Resuming += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &App::OnResuming);
}

anim::App::~App()
{
}

anim::App ^anim::App::Current::get()
{
	return safe_cast<App ^>(Windows::UI::Xaml::Application::Current);
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
}

void anim::App::InitializeWindow(Windows::UI::Xaml::Window ^window)
{
	ViewAppState ^viewState = ref new ViewAppState(&state);
	MainPage ^page = ref new MainPage(viewState);
	window->Content = page;

	window->Activate();
}

void anim::App::OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args)
{
}

void anim::App::OnResuming(Platform::Object ^sender, Platform::Object ^arg)
{
}
