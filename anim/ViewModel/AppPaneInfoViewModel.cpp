#include "pch.h"
#include "Core/String.h"
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
	, active(false)
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
			break;

		case AppPaneType::None:
			this->name = "<none>";
			break;

		case AppPaneType::Files:
			this->name = Resource::GetString("FilesPaneName");
			break;

		case AppPaneType::Color:
			this->name = Resource::GetString("ColorPaneName");
			break;

		case AppPaneType::Layers:
			this->name = Resource::GetString("LayersPaneName");
			break;

		case AppPaneType::View:
			this->name = Resource::GetString("ViewPaneName");
			break;

		case AppPaneType::Animation:
			this->name = Resource::GetString("AnimationPaneName");
			break;
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
			break;

		case AppPaneType::Color:
			uri = "ms-appx:///Assets/Icons/ColorPalette.png";
			break;

		case AppPaneType::Layers:
			uri = "ms-appx:///Assets/Icons/Layers.png";
			break;
	
		case AppPaneType::View:
			uri = "ms-appx:///Assets/Icons/Zoom.png";
			break;

		case AppPaneType::Animation:
			uri = "ms-appx:///Assets/Icons/Animation.png";
			break;
		}

		if (uri != nullptr)
		{
			this->icon = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(
				ref new Windows::Foundation::Uri(uri));
		}
	}

	return this->icon;
}

Windows::UI::Xaml::UIElement ^anim::AppPaneInfoViewModel::Pane::get()
{
	if (this->pane == nullptr && this->parent != nullptr)
	{
		this->pane = this->parent->CreatePane();
	}

	return this->pane;
}

bool anim::AppPaneInfoViewModel::IsActive::get()
{
	return this->active;
}

void anim::AppPaneInfoViewModel::IsActive::set(bool value)
{
	if (this->active != value)
	{
		this->active = value;
		this->NotifyPropertyChanged("IsActive");
	}
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
