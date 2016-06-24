#include "pch.h"
#include "App.xaml.h"
#include "MainPage.xaml.h"

png_editor::App::App()
{
	this->InitializeComponent();

	this->Suspending += ref new Windows::UI::Xaml::SuspendingEventHandler(this, &App::OnSuspending);
	this->Resuming += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &App::OnResuming);
}

png_editor::App::~App()
{
}

png_editor::App ^png_editor::App::Current::get()
{
	return safe_cast<App ^>(Application::Current);
}

void png_editor::App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs ^args)
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

void png_editor::App::InitializeProcess()
{
}

void png_editor::App::InitializeGlobals()
{
}

void png_editor::App::InitializeWindow(Windows::UI::Xaml::Window ^window)
{
	MainPage ^page = ref new MainPage();
	window->Content = page;

	window->Activate();
}

void png_editor::App::OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args)
{
}

void png_editor::App::OnResuming(Platform::Object ^sender, Platform::Object ^arg)
{
}
