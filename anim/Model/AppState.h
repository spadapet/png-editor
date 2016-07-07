#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState
	{
	public:
		AppState();
		~AppState();

		SimpleEvent Disposed;
		ChangedEvent PropertyChanged;

	private:
	};
}
