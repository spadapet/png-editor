#pragma once

#include "Converters/BoolToBrushConverter.h"
#include "Converters/BoolToVisibleConverter.h"
#include "UI/AppShell.g.h"
#include "ViewModel/PaneInfoViewModel.h"
#include "ViewModel/ShellViewModel.h"

namespace anim
{
	public ref class AppShell sealed
	{
	public:
		AppShell();
		virtual ~AppShell();

		property ShellViewModel ^State { ShellViewModel ^get(); }

	private:
		ShellViewModel ^state;
	};
}
