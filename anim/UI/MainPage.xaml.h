#pragma once

#include "UI/MainPage.g.h"
#include "ViewModel/MainPageViewModel.h"

namespace anim
{
	class AppState;
	ref class MainPageViewModel;

	public ref class MainPage sealed
	{
	internal:
		MainPage(AppState *app);

	public:
		virtual ~MainPage();

		property MainPageViewModel ^State { MainPageViewModel ^get(); }

	private:
		MainPageViewModel ^state;
	};
}
