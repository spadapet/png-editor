#include "pch.h"
#include "App.xaml.h"
#include "Controller/AppPersist.h"
#include "Core/Thread.h"
#include "View/MainPage.xaml.h"

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
			this->InitializeGlobals().then([this]()
			{
				this->state->SetDpi(this->displayInfo->LogicalDpi);
				this->InitializeWindow(Windows::UI::Xaml::Window::Current);
			}, concurrency::task_continuation_context::use_current());
		}
		break;
	}
}

void anim::App::InitializeProcess()
{
	anim::SetMainThread();

	if (this->displayInfo == nullptr)
	{
		this->displayInfo = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
		this->dpiChangedToken = this->displayInfo->DpiChanged +=
			ref new Windows::Foundation::TypedEventHandler<Windows::Graphics::Display::DisplayInformation ^, Platform::Object ^>(
				this, &App::OnDpiChanged);
	}
}

concurrency::task<void> anim::App::InitializeGlobals()
{
	auto loadTask = anim::LoadAppState();

	auto doneTask = loadTask.then([this](std::shared_ptr<AppState> app)
	{
		this->state = app;
	});

	return doneTask;
}

void anim::App::InitializeWindow(Windows::UI::Xaml::Window ^window)
{
	window->Content = ref new MainPage(this->state);
	window->Activate();
}

void anim::App::OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args)
{
	Windows::ApplicationModel::SuspendingDeferral ^deferral = args->SuspendingOperation->GetDeferral();
	auto saveTask = anim::SaveAppState(this->state);

	saveTask.then([deferral]()
	{
		deferral->Complete();
	}, concurrency::task_continuation_context::use_current());
}

void anim::App::OnResuming(Platform::Object ^sender, Platform::Object ^arg)
{
}

void anim::App::OnDpiChanged(Windows::Graphics::Display::DisplayInformation ^displayInfo, Platform::Object ^args)
{
	if (this->state != nullptr)
	{
		this->state->SetDpi(this->displayInfo->LogicalDpi);
	}
}
