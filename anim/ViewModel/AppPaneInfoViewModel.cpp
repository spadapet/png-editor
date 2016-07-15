#include "pch.h"
#include "Model/AppPaneInfo.h"
#include "ViewModel/AppPaneInfoViewModel.h"

anim::AppPaneInfoViewModel::AppPaneInfoViewModel()
	: AppPaneInfoViewModel(nullptr)
{
}

anim::AppPaneInfoViewModel::AppPaneInfoViewModel(AppPaneInfo *parent)
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

anim::AppPaneInfoViewModel::~AppPaneInfoViewModel()
{
	if (this->parent != nullptr)
	{
		this->parent->Disposed.Remove(this->parentDisposedCookie);
	}
}

Platform::String ^anim::AppPaneInfoViewModel::Name::get()
{
	return "Pane name";
}

void anim::AppPaneInfoViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
