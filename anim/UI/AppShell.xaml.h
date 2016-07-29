#pragma once

#include "Converters/BoolToBrushConverter.h"
#include "Converters/BoolToVisibleConverter.h"
#include "UI/AppShell.g.h"
#include "ViewModel/PaneInfoVM.h"
#include "ViewModel/ShellVM.h"

namespace anim
{
	class AppState;

	public ref class AppShell sealed
	{
	internal:
		AppShell(AppState *app);

	public:
		virtual ~AppShell();

		property ShellVM ^State { ShellVM ^get(); }

	private:
		ShellVM ^state;
	};
}
