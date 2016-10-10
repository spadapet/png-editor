#pragma once

#include "Core/Event.h"

namespace anim
{
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		ChangedEvent PropertyChanged;
	};
}
