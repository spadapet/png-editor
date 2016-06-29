#include "pch.h"
#include "ViewModel/ViewAppState.h"

anim::ViewAppState::ViewAppState(AppState *parent)
	: parent(parent)
{
	assert(parent != nullptr);
}

anim::ViewAppState::~ViewAppState()
{
}
