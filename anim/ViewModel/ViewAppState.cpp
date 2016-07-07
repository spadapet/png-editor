#include "pch.h"
#include "Model/AppState.h"
#include "ViewModel/ViewAppState.h"

anim::ViewAppState::ViewAppState(AppState *parent)
	: parent(parent)
	, changedCookie(NULL_EVENT_COOKIE)
{
	assert(parent != nullptr);

	this->changedCookie = this->parent->PropertyChanged.Add([this](const char *name)
	{
		this->ModelPropertyChanged(name);
	});
}

anim::ViewAppState::~ViewAppState()
{
	if (this->parent != nullptr && this->changedCookie != NULL_EVENT_COOKIE)
	{
		this->parent->PropertyChanged.Remove(this->changedCookie);
		this->changedCookie = NULL_EVENT_COOKIE;
	}
}

void anim::ViewAppState::ModelPropertyChanged(const char *name)
{
}
