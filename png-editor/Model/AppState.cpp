#include "pch.h"
#include "Model/AppState.h"
#include "ViewModel/ViewAppState.h"

AppState::AppState()
{
	this->viewState = ref new  png_editor::ViewAppState(this);
}

AppState::~AppState()
{
}

png_editor::ViewAppState ^AppState::GetViewModel() const
{
	return this->viewState;
}
