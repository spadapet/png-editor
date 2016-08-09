#pragma once

#include "View/MainPage.g.h"
#include "ViewModel/MainPageVM.h"

namespace anim
{
	class AppState;

	public ref class MainPage sealed
	{
	internal:
		MainPage(std::shared_ptr<AppState> app);

	public:
		MainPage();
		virtual ~MainPage();

		property MainPageVM ^State { MainPageVM ^get(); }

	private:
		MainPageVM ^state;
	};
}
