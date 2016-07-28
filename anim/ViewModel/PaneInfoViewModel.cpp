#include "pch.h"
#include "App.xaml.h"
#include "Core/Command.h"
#include "Core/String.h"
#include "Model/PaneInfo.h"
#include "Model/AppState.h"
#include "ViewModel/PaneInfoViewModel.h"
#include "ViewModel/ShellViewModel.h"

anim::PaneInfoViewModel::PaneInfoViewModel()
	: PaneInfoViewModel(nullptr, nullptr, nullptr)
{
}

anim::PaneInfoViewModel::PaneInfoViewModel(AppState *app, PaneInfo *pane, ShellViewModel ^shell)
	: app(app)
	, pane(pane)
	, appDisposedCookie(NULL_EVENT_COOKIE)
	, appChangedCookie(NULL_EVENT_COOKIE)
	, paneDisposedCookie(NULL_EVENT_COOKIE)
	, paneChangedCookie(NULL_EVENT_COOKIE)
	, active(false)
{
	if (this->pane == nullptr || this->app == nullptr)
	{
		// Probably running in XAML designer
		return;
	}

	Platform::WeakReference weakThis(this);
	Platform::WeakReference weakShell(shell);

	this->appDisposedCookie = this->app->Destroyed.Add([weakThis]()
	{
		auto owner = weakThis.Resolve<PaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->app = nullptr;
			owner->NotifyPropertyChanged();
		}
	});

	this->appDisposedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<PaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});

	this->paneDisposedCookie = this->pane->Destroyed.Add([weakThis]()
	{
		auto owner = weakThis.Resolve<PaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->pane = nullptr;
			owner->NotifyPropertyChanged();
		}
	});

	this->paneChangedCookie = this->pane->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<PaneInfoViewModel>();
		if (owner != nullptr)
		{
			owner->ModelPropertyChanged(name);
		}
	});

	this->toggleActiveCommand = ref new Command([weakThis, weakShell](Platform::Object ^)
	{
		auto owner = weakThis.Resolve<PaneInfoViewModel>();
		auto shell = weakShell.Resolve<ShellViewModel>();

		if (owner != nullptr && shell != nullptr)
		{
			owner->ToggleActive(shell);
		}
	});
}

anim::PaneInfoViewModel::~PaneInfoViewModel()
{
	if (this->app != nullptr)
	{
		this->app->Destroyed.Remove(this->appDisposedCookie);
	}

	if (this->pane != nullptr)
	{
		this->pane->Destroyed.Remove(this->paneDisposedCookie);
		this->pane->PropertyChanged.Remove(this->paneChangedCookie);
	}
}

Platform::String ^anim::PaneInfoViewModel::Name::get()
{
	if (this->name == nullptr)
	{
		switch (pane != nullptr ? pane->GetType() : PaneType::None)
		{
		default:
			assert(false);
			this->name = "<invalid>";
			break;

		case PaneType::None:
			this->name = "<none>";
			break;

		case PaneType::Files:
			this->name = Resource::GetString("FilesPaneName");
			break;

		case PaneType::Color:
			this->name = Resource::GetString("ColorPaneName");
			break;

		case PaneType::Layers:
			this->name = Resource::GetString("LayersPaneName");
			break;

		case PaneType::View:
			this->name = Resource::GetString("ViewPaneName");
			break;

		case PaneType::Animation:
			this->name = Resource::GetString("AnimationPaneName");
			break;
		}
	}

	return this->name;
}

Windows::UI::Xaml::Media::ImageSource ^anim::PaneInfoViewModel::Icon::get()
{
	if (this->icon == nullptr)
	{
		Platform::String ^uri = nullptr;

		switch (this->pane != nullptr ? this->pane->GetType() : PaneType::None)
		{
		default:
			assert(false);
			break;

		case PaneType::None:
			break;

		case PaneType::Files:
			uri = "ms-appx:///Assets/Icons/FileGroup.png";
			break;

		case PaneType::Color:
			uri = "ms-appx:///Assets/Icons/ColorPalette.png";
			break;

		case PaneType::Layers:
			uri = "ms-appx:///Assets/Icons/Layers.png";
			break;
	
		case PaneType::View:
			uri = "ms-appx:///Assets/Icons/Zoom.png";
			break;

		case PaneType::Animation:
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

Windows::UI::Xaml::UIElement ^anim::PaneInfoViewModel::Pane::get()
{
	if (this->paneUi == nullptr && this->pane != nullptr)
	{
		this->paneUi = this->pane->CreatePane();
	}

	return this->paneUi;
}

Windows::UI::Xaml::Input::ICommand ^anim::PaneInfoViewModel::ToggleActiveCommand::get()
{
	return this->toggleActiveCommand;
}

bool anim::PaneInfoViewModel::IsActive::get()
{
	return this->active;
}

void anim::PaneInfoViewModel::IsActive::set(bool value)
{
	if (this->active != value)
	{
		this->active = value;
		this->NotifyPropertyChanged("IsActive");
	}
}

bool anim::PaneInfoViewModel::IsVisible::get()
{
	bool visible = false;

	switch (this->pane != nullptr ? this->pane->GetType() : PaneType::None)
	{
	default:
	case PaneType::None:
		break;

	case PaneType::Files:
		visible = true;
		break;

	case PaneType::Color:
	case PaneType::Layers:
	case PaneType::View:
	case PaneType::Animation:
#ifdef _DEBUG
		visible = true;
#else
		visible = this->appState && (this->appState->GetMode() == AppMode::Edit);
#endif
		break;
	}

	return visible;
}

void anim::PaneInfoViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::PaneInfoViewModel::AppPropertyChanged(const char *name)
{
	if (strcmp(name, "Mode") == 0)
	{
		this->NotifyPropertyChanged("IsVisible");
	}
}

void anim::PaneInfoViewModel::ModelPropertyChanged(const char *name)
{
}

void anim::PaneInfoViewModel::ToggleActive(ShellViewModel ^shell)
{
	shell->ActivePane = (shell->ActivePane != this) ? this : nullptr;
}
