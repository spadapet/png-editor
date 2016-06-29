#pragma once

#include "MainPage.g.h"

class AppState;

namespace anim
{
	ref class ViewAppState;

	public ref class MainPage sealed
	{
	internal:
		MainPage(ViewAppState ^state);

	public:
		MainPage();
		virtual ~MainPage();

		property ViewAppState ^State { ViewAppState ^get(); }

	private:
		ViewAppState ^state;
	};
}
