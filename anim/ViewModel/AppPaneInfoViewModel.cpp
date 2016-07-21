#include "pch.h"
#include "Core/Resource.h"
#include "Model/AppPaneInfo.h"
#include "ViewModel/AppPaneInfoViewModel.h"

anim::AppPaneInfoViewModel::AppPaneInfoViewModel()
	: AppPaneInfoViewModel(nullptr)
{
}

anim::AppPaneInfoViewModel::AppPaneInfoViewModel(AppPaneInfo *parent)
	: parent(parent)
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
{
	if (this->parent == nullptr)
	{
		return;
	}

	this->parentDisposedCookie = this->parent->Disposed.Add([this]()
	{
		this->parent = nullptr;
		this->NotifyPropertyChanged();
	});

	this->parentChangedCookie = this->parent->PropertyChanged.Add([this](const char *name)
	{
		this->ModelPropertyChanged(name);
	});
}

anim::AppPaneInfoViewModel::~AppPaneInfoViewModel()
{
	if (this->parent != nullptr)
	{
		this->parent->Disposed.Remove(this->parentDisposedCookie);
		this->parent->PropertyChanged.Remove(this->parentChangedCookie);
	}
}

Platform::String ^anim::AppPaneInfoViewModel::Name::get()
{
	if (this->name == nullptr)
	{
		switch (parent != nullptr ? parent->GetType() : AppPaneType::None)
		{
		default:
			assert(false);
			this->name = "<invalid>";

		case AppPaneType::None:
			this->name = "<none>";

		case AppPaneType::Files:
			this->name = Resource::GetString("FilesPaneName");
		}
	}

	return this->name;
}

Windows::UI::Xaml::Media::ImageSource ^anim::AppPaneInfoViewModel::Icon::get()
{
	if (this->icon == nullptr)
	{
		Platform::String ^uri = nullptr;

		switch (this->parent != nullptr ? this->parent->GetType() : AppPaneType::None)
		{
		default:
			assert(false);
			break;

		case AppPaneType::None:
			break;

		case AppPaneType::Files:
			uri = "ms-appx:///Assets/Icons/FileGroup.png";
		}

		if (uri != nullptr)
		{
			this->icon = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(
				ref new Windows::Foundation::Uri(uri));
		}
	}

	return this->icon;
}

void anim::AppPaneInfoViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::AppPaneInfoViewModel::ModelPropertyChanged(const char *name)
{
	if (strcmp(name, "Type") == 0)
	{
		this->NotifyPropertyChanged("Name");
		this->NotifyPropertyChanged("Icon");
	}
}
