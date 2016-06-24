#pragma once

#include "App.g.h"

namespace png_editor
{
	ref class App sealed
	{
	public:
		App();
		virtual ~App();
		static property App ^Current { App ^get(); }

		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs ^args) override;

	private:
		void InitializeProcess();
		void InitializeGlobals();
		void InitializeWindow(Windows::UI::Xaml::Window ^window);

		void OnSuspending(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
		void OnResuming(Platform::Object ^sender, Platform::Object ^arg);
	};
}
