#pragma once

#include "Core/Properties.h"
#include "UI/Shell.g.h"
#include "UI/Utility/BoolToBrushConverter.h"
#include "UI/Utility/BoolToVisibleConverter.h"
#include "ViewModel/PaneInfoVM.h"
#include "ViewModel/ShellVM.h"

namespace anim
{
	class AppState;

	public ref class Shell sealed
	{
	internal:
		Shell(std::shared_ptr<AppState> app);

	public:
		Shell();
		virtual ~Shell();

		property ShellVM ^State { ShellVM ^get(); }

	private:
		ShellVM ^state;
	};
}
