#pragma once

#include "UI/FilesPane.g.h"

namespace anim
{
	class AppState;
	ref class ShellViewModel;

	public ref class FilesPane sealed
	{
	internal:
		FilesPane(AppState *app);

	private:
		AppState *app;
	};
}
