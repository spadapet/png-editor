#pragma once

#include "Converters/BoolToBrushConverter.h"
#include "Converters/BoolToVisibleConverter.h"
#include "UI/AppShell.g.h"
#include "ViewModel/AppPaneInfoViewModel.h"
#include "ViewModel/AppShellViewModel.h"

namespace anim
{
	ref class AppShellViewModel;

	public ref class AppShell sealed
	{
	public:
		AppShell();
		virtual ~AppShell();

		property AppShellViewModel ^State { AppShellViewModel ^get(); }

	private:
		AppShellViewModel ^state;
	};
}
