#pragma once

#include "UI/MainPage.g.h"

class AppState;

namespace anim
{
	ref class MainPageState;

	public ref class MainPage sealed
	{
	public:
		MainPage();
		virtual ~MainPage();

		property MainPageState ^State { MainPageState ^get(); }

	private:
		MainPageState ^state;
	};
}
