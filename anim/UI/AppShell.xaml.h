#pragma once

#include "UI/AppShell.g.h"

namespace anim
{
	ref class AppShellState;

	public ref class AppShell sealed
	{
	public:
		AppShell();
		virtual ~AppShell();

		property AppShellState ^State { AppShellState ^get(); }

	private:
		AppShellState ^state;
	};
}
