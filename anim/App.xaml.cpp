#include "pch.h"
#include "App.xaml.h"
#include "UI/MainPage.xaml.h"
#include "ViewModel/ShellViewModel.h"

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

anim::AppState &anim::App::GetGlobalState()
{
	return this->state;
}

const anim::AppState &anim::App::GetGlobalState() const
{
	return this->state;
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
			Platform::WeakReference weakThis(this);

			auto initGlobals = concurrency::task<void>([weakThis]()
			{
				App ^owner = weakThis.Resolve<App>();
				if (owner != nullptr)
				{
					owner->InitializeGlobals();
				}
			});

			initGlobals.then([weakThis]()
			{
				App ^owner = weakThis.Resolve<App>();
				if (owner != nullptr)
				{
					owner->InitializeWindow(Windows::UI::Xaml::Window::Current);
				}
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
	window->Content = ref new MainPage();
	window->Activate();
}

void anim::App::OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args)
{
}

void anim::App::OnResuming(Platform::Object ^sender, Platform::Object ^arg)
{
}
