#pragma once

#include "UI/MainPage.g.h"

class AppState;

namespace anim
{
	ref class MainPageViewModel;

	public ref class MainPage sealed
	{
	public:
		MainPage();
		virtual ~MainPage();

		property MainPageViewModel ^State { MainPageViewModel ^get(); }

	private:
		MainPageViewModel ^state;
	};
}
