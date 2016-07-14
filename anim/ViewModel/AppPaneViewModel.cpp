#include "pch.h"
#include "Model/AppPane.h"
#include "ViewModel/AppPaneViewModel.h"

anim::AppPaneViewModel::AppPaneViewModel()
	: AppPaneViewModel(nullptr)
{
}

anim::AppPaneViewModel::AppPaneViewModel(AppPane *parent)
	: parent(parent)
	, parentDisposedCookie(NULL_EVENT_COOKIE)
{
	if (this->parent == nullptr)
	{
		return;
	}

	this->parentDisposedCookie = this->parent->Disposed.Add([this]()
	{
		this->parent = nullptr;
	});
}

anim::AppPaneViewModel::~AppPaneViewModel()
{
	if (this->parent != nullptr)
	{
		this->parent->Disposed.Remove(this->parentDisposedCookie);
	}
}

Platform::String ^anim::AppPaneViewModel::Name::get()
{
	return "Pane name";
}

void anim::AppPaneViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
