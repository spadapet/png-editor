﻿#pragma once

#include "App.g.h"
#include "Model/AppState.h"

namespace anim
{
	ref class App sealed
	{
	public:
		App();

		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs ^args) override;

	private:
		void InitializeProcess();
		void InitializeGlobals();
		void InitializeWindow(Windows::UI::Xaml::Window ^window);

		void OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
		void OnResuming(Platform::Object ^sender, Platform::Object ^arg);

		AppState state;
	};
}
