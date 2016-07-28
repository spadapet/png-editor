#pragma once

#include "UI/MainPage.g.h"

namespace anim
{
	class AppState;
	ref class AppShell;
	ref class MainPageViewModel;

	public ref class MainPage sealed
	{
	internal:
		MainPage(AppState *app);

	public:
		virtual ~MainPage();

		property MainPageViewModel ^State { MainPageViewModel ^get(); }
		property Windows::UI::Xaml::UIElement ^Shell { Windows::UI::Xaml::UIElement ^get(); }

	private:
		MainPageViewModel ^state;
		AppShell ^shell;
	};
}
