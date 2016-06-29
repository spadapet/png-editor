#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState
	{
	public:
		AppState();
		~AppState();

		ChangedEvent PropertyChanged;

	private:
	};
}
