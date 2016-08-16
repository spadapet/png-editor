#pragma once

#include "Core/Properties.h"
#include "View/Shell.g.h"
#include "View/Utility/BoolToObjectConverter.h"
#include "View/Utility/BoolToVisibleConverter.h"
#include "ViewModel/ShellVM.h"

namespace anim
{
	class AppState;

	[Windows::UI::Xaml::Data::Bindable]
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
