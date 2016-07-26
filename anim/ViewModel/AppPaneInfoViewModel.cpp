#include "pch.h"
#include "App.xaml.h"
#include "Core/Command.h"
#include "Core/String.h"
#include "Model/AppPaneInfo.h"
#include "Model/AppState.h"
#include "ViewModel/AppPaneInfoViewModel.h"
#include "ViewModel/AppShellViewModel.h"

anim::AppPaneInfoViewModel::AppPaneInfoViewModel()
	: AppPaneInfoViewModel(nullptr, nullptr)
{
}

anim::AppPaneInfoViewModel::AppPaneInfoViewModel(AppPaneInfo *parent, AppShellViewModel ^shellViewModel)
	: parent(parent)
	, appState(&App::Current->GetGlobalState())
	, appStateDisposedCookie(NULL_EVENT_COOKIE)
	, appStateChangedCookie(NULL_EVENT_COOKIE)
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
	, shellViewModel(shellViewModel)
	, active(false)
{
	Platform::WeakReference weakThis(this);

	if (this->parent == nullptr)
	{
		return;
	}

	this->appStateDisposedCookie = this->appState->Disposed.Add([weakThis]()
	{
		auto owner = weakThis.Resolve<AppPaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->appState = nullptr;
			owner->NotifyPropertyChanged();
		}
	});

	this->appStateDisposedCookie = this->appState->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<AppPaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});

	this->parentDisposedCookie = this->parent->Disposed.Add([weakThis]()
	{
		auto owner = weakThis.Resolve<AppPaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->parent = nullptr;
			owner->NotifyPropertyChanged();
		}
	});

	this->parentChangedCookie = this->parent->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<AppPaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->ModelPropertyChanged(name);
		}
	});

	this->toggleActiveCommand = ref new Command([weakThis](Platform::Object ^)
	{
		auto owner = weakThis.Resolve<AppPaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->ToggleActive();
		}
	});
}

anim::AppPaneInfoViewModel::~AppPaneInfoViewModel()
{
	if (this->appState != nullptr)
	{
		this->appState->Disposed.Remove(this->appStateDisposedCookie);
	}

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

Windows::UI::Xaml::Input::ICommand ^anim::AppPaneInfoViewModel::ToggleActiveCommand::get()
{
	return this->toggleActiveCommand;
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

bool anim::AppPaneInfoViewModel::IsVisible::get()
{
	bool visible = false;

	switch (this->parent != nullptr ? this->parent->GetType() : AppPaneType::None)
	{
	default:
	case AppPaneType::None:
		break;

	case AppPaneType::Files:
		visible = true;
		break;

	case AppPaneType::Color:
	case AppPaneType::Layers:
	case AppPaneType::View:
	case AppPaneType::Animation:
		visible = this->appState && (this->appState->GetMode() == AppMode::Edit);
		break;
	}

	return visible;
}

void anim::AppPaneInfoViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::AppPaneInfoViewModel::AppPropertyChanged(const char *name)
{
	if (strcmp(name, "Mode") == 0)
	{
		this->NotifyPropertyChanged("IsVisible");
	}
}

void anim::AppPaneInfoViewModel::ModelPropertyChanged(const char *name)
{
}

void anim::AppPaneInfoViewModel::ToggleActive()
{
	AppShellViewModel ^shellViewModel = this->shellViewModel.Resolve<AppShellViewModel>();
	if (shellViewModel != nullptr)
	{
		shellViewModel->ActivePane = (shellViewModel->ActivePane != this)
			? this
			: nullptr;
	}
}
