#include "pch.h"
#include "Model/AppState.h"
#include "ViewModel/ViewAppState.h"

anim::ViewAppState::ViewAppState(AppState *parent)
	: parent(parent)
	, modelChangedCookie(0)
{
	assert(parent != nullptr);

	this->modelChangedCookie = this->parent->PropertyChanged.Add([this](const char *name)
	{
		this->ModelPropertyChanged(name);
	});
}

anim::ViewAppState::~ViewAppState()
{
	if (this->parent != nullptr && this->modelChangedCookie != 0)
	{
		this->parent->PropertyChanged.Remove(this->modelChangedCookie);
		this->modelChangedCookie = 0;
	}
}

void anim::ViewAppState::ModelPropertyChanged(const char *name)
{
}
