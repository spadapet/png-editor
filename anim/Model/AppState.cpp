#include "pch.h"
#include "Model/AppState.h"

anim::AppState::AppState()
{
}

anim::AppState::~AppState()
{
	Disposed.Notify();
}
