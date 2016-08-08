#include "pch.h"
#include "App.xaml.h"
#include "Core/Command.h"
#include "Core/String.h"
#include "Model/PaneInfo.h"
#include "Model/AppState.h"
#include "ViewModel/PaneInfoVM.h"
#include "ViewModel/ShellVM.h"

anim::PaneInfoVM::PaneInfoVM(std::shared_ptr<AppState> app, std::shared_ptr<PaneInfo> pane, ShellVM ^shell)
	: app(app)
	, pane(pane)
	, appChangedCookie(NULL_EVENT_COOKIE)
	, paneChangedCookie(NULL_EVENT_COOKIE)
	, active(false)
{
	Platform::WeakReference weakThis(this);
	Platform::WeakReference weakShell(shell);

	this->appChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		PaneInfoVM ^owner = weakThis.Resolve<PaneInfoVM>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});

	this->paneChangedCookie = this->pane->PropertyChanged.Add([weakThis](const char *name)
	{
		PaneInfoVM ^owner = weakThis.Resolve<PaneInfoVM>();
		if (owner != nullptr)
		{
			owner->PanePropertyChanged(name);
		}
	});

	this->toggleActiveCommand = ref new Command([weakThis, weakShell](Platform::Object ^)
	{
		PaneInfoVM ^owner = weakThis.Resolve<PaneInfoVM>();
		ShellVM ^shell = weakShell.Resolve<ShellVM>();

		if (owner != nullptr && shell != nullptr)
		{
			owner->ToggleActive(shell);
		}
	});
}

anim::PaneInfoVM::PaneInfoVM(std::shared_ptr<AppState> app)
	: PaneInfoVM(app, app->GetPanes().empty() ? app->GetNonePane() : app->GetPanes()[0], ref new ShellVM(app))
{
}

anim::PaneInfoVM::PaneInfoVM()
	: PaneInfoVM(AppState::CreateForDesigner())
{
}

anim::PaneInfoVM::~PaneInfoVM()
{
	this->pane->PropertyChanged.Remove(this->paneChangedCookie);
}

Platform::String ^anim::PaneInfoVM::Name::get()
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
			this->name = Resource::GetString("/Resources/FilesPaneName/Text");
			break;

		case PaneType::Color:
			this->name = Resource::GetString("/Resources/ColorPaneName/Text");
			break;

		case PaneType::Layers:
			this->name = Resource::GetString("/Resources/LayersPaneName/Text");
			break;

		case PaneType::View:
			this->name = Resource::GetString("/Resources/ViewPaneName/Text");
			break;

		case PaneType::Animation:
			this->name = Resource::GetString("/Resources/AnimationPaneName/Text");
			break;
		}
	}

	return this->name;
}

Windows::UI::Xaml::Media::ImageSource ^anim::PaneInfoVM::Icon::get()
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
			uri = "ms-appx:///Assets/Icons/documents.png";
			break;

		case PaneType::Color:
			uri = "ms-appx:///Assets/Icons/image_colors.png";
			break;

		case PaneType::Layers:
			uri = "ms-appx:///Assets/Icons/layers.png";
			break;
	
		case PaneType::View:
			uri = "ms-appx:///Assets/Icons/view_zoom.png";
			break;

		case PaneType::Animation:
			uri = "ms-appx:///Assets/Icons/movie2.png";
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

Windows::UI::Xaml::UIElement ^anim::PaneInfoVM::Pane::get()
{
	if (this->paneUi == nullptr && this->pane != nullptr)
	{
		this->paneUi = this->pane->CreatePane();
	}

	return this->paneUi;
}

Windows::UI::Xaml::Input::ICommand ^anim::PaneInfoVM::ToggleActiveCommand::get()
{
	return this->toggleActiveCommand;
}

bool anim::PaneInfoVM::IsActive::get()
{
	return this->active;
}

void anim::PaneInfoVM::IsActive::set(bool value)
{
	if (this->active != value)
	{
		this->active = value;
		this->NotifyPropertyChanged("IsActive");
	}
}

bool anim::PaneInfoVM::IsVisible::get()
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
		visible = this->app && (this->app->GetMode() == AppMode::Edit);
#endif
		break;
	}

	return visible;
}

void anim::PaneInfoVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::PaneInfoVM::AppPropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);

	if (allChanged || strcmp(name, "Mode") == 0)
	{
		this->NotifyPropertyChanged("IsVisible");
	}
}

void anim::PaneInfoVM::PanePropertyChanged(const char *name)
{
}

void anim::PaneInfoVM::ToggleActive(ShellVM ^shell)
{
	shell->ActivePane = (shell->ActivePane != this) ? this : nullptr;
}
