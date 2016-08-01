#pragma once

#include "Converters/BoolToBrushConverter.h"
#include "Converters/BoolToVisibleConverter.h"
#include "UI/Shell.g.h"
#include "ViewModel/PaneInfoVM.h"
#include "ViewModel/ShellVM.h"

namespace anim
{
	class AppState;

	public ref class Shell sealed
	{
	internal:
		Shell(AppState *app);

	public:
		virtual ~Shell();

		property ShellVM ^State { ShellVM ^get(); }

	private:
		ShellVM ^state;
	};
}
