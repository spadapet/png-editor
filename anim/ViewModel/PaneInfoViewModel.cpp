#include "pch.h"
#include "App.xaml.h"
#include "Core/Command.h"
#include "Core/String.h"
#include "Model/AppPaneInfo.h"
#include "Model/AppState.h"
#include "ViewModel/PaneInfoViewModel.h"
#include "ViewModel/ShellViewModel.h"

anim::PaneInfoViewModel::PaneInfoViewModel()
	: PaneInfoViewModel(nullptr, nullptr, nullptr)
{
}

anim::PaneInfoViewModel::PaneInfoViewModel(AppState *app, AppPaneInfo *pane, ShellViewModel ^shell)
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

	this->appDisposedCookie = this->app->Disposed.Add([weakThis]()
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

	this->paneDisposedCookie = this->pane->Disposed.Add([weakThis]()
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
		this->app->Disposed.Remove(this->appDisposedCookie);
	}

	if (this->pane != nullptr)
	{
		this->pane->Disposed.Remove(this->paneDisposedCookie);
		this->pane->PropertyChanged.Remove(this->paneChangedCookie);
	}
}

Platform::String ^anim::PaneInfoViewModel::Name::get()
{
	if (this->name == nullptr)
	{
		switch (pane != nullptr ? pane->GetType() : AppPaneType::None)
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

Windows::UI::Xaml::Media::ImageSource ^anim::PaneInfoViewModel::Icon::get()
{
	if (this->icon == nullptr)
	{
		Platform::String ^uri = nullptr;

		switch (this->pane != nullptr ? this->pane->GetType() : AppPaneType::None)
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

	switch (this->pane != nullptr ? this->pane->GetType() : AppPaneType::None)
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
