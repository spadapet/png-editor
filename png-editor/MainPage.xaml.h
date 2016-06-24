#pragma once

#include "MainPage.g.h"

class AppState;

namespace png_editor
{
	ref class ViewAppState;

	public ref class MainPage sealed
	{
	internal:
		MainPage(AppState *state);

	public:
		MainPage();

		property ViewAppState ^State { ViewAppState ^get(); }
	};
}
