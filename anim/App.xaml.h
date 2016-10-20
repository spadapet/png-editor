#pragma once

#include "App.g.h"
#include "Model/AppState.h"

// Not sure why these view models need to be included globally,
// but the build fails without them.
#include "ViewModel/PaneInfoVM.h"
#include "ViewModel/ProjectFileVM.h"
#include "ViewModel/ProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

namespace anim
{
	ref class App sealed
	{
	public:
		App();

		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs ^args) override;

	private:
		void InitializeProcess();
		concurrency::task<void> InitializeGlobals();
		void InitializeWindow(Windows::UI::Xaml::Window ^window);

		void OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
		void OnResuming(Platform::Object ^sender, Platform::Object ^arg);
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation ^displayInfo, Platform::Object ^args);

		std::shared_ptr<AppState> state;
		Windows::Graphics::Display::DisplayInformation ^displayInfo;
		Windows::Foundation::EventRegistrationToken dpiChangedToken;
	};
}
