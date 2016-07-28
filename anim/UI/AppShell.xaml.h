#pragma once

#include "Converters/BoolToBrushConverter.h"
#include "Converters/BoolToVisibleConverter.h"
#include "UI/AppShell.g.h"
#include "ViewModel/PaneInfoViewModel.h"
#include "ViewModel/ShellViewModel.h"

namespace anim
{
	class AppState;

	public ref class AppShell sealed
	{
	internal:
		AppShell(AppState *app);

	public:
		virtual ~AppShell();

		property ShellViewModel ^State { ShellViewModel ^get(); }

	private:
		ShellViewModel ^state;
	};
}
