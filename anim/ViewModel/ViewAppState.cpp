#include "pch.h"
#include "Model/AppState.h"
#include "ViewModel/ViewAppState.h"

anim::ViewAppState::ViewAppState(AppState *parent)
	: parent(parent)
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
{
	assert(parent != nullptr);

	this->parentDisposedCookie = this->parent->Disposed.Add([this]()
	{
		this->parent = nullptr;
	});

	this->parentChangedCookie = this->parent->PropertyChanged.Add([this](const char *name)
	{
		this->ModelPropertyChanged(name);
	});
}

anim::ViewAppState::~ViewAppState()
{
	if (this->parent != nullptr)
	{
		this->parent->PropertyChanged.Remove(this->parentChangedCookie);
		this->parent->Disposed.Remove(this->parentDisposedCookie);
	}
}

void anim::ViewAppState::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ViewAppState::ModelPropertyChanged(const char *name)
{
}
