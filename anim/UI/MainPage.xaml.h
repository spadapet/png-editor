#pragma once

#include "UI/MainPage.g.h"
#include "ViewModel/MainPageVM.h"

namespace anim
{
	class AppState;

	public ref class MainPage sealed
	{
	internal:
		MainPage(AppState *app);

	public:
		virtual ~MainPage();

		property MainPageVM ^State { MainPageVM ^get(); }

	private:
		MainPageVM ^state;
	};
}
