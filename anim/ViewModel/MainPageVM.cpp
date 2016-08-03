#include "pch.h"
#include "Model/AppState.h"
#include "UI/Shell.xaml.h"
#include "ViewModel/MainPageVM.h"

anim::MainPageVM::MainPageVM(std::shared_ptr<AppState> app)
	: app(app)
	, appChangedCookie(NULL_EVENT_COOKIE)
{
	if (this->app == nullptr)
	{
		// Probably running in XAML desigher
		return;
	}

	Platform::WeakReference weakThis(this);

	this->appChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<MainPageVM>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});
}

anim::MainPageVM::MainPageVM()
	: MainPageVM(AppState::CreateForDesigner())
{
}

anim::MainPageVM::~MainPageVM()
{
	if (this->app != nullptr)
	{
		this->app->PropertyChanged.Remove(this->appChangedCookie);
	}
}

Windows::UI::Xaml::UIElement ^anim::MainPageVM::UserInterface::get()
{
	Windows::UI::Xaml::UIElement ^ui = nullptr;

	if (this->app != nullptr)
	{
		switch (this->app->GetMode())
		{
		case AppMode::None:
		case AppMode::Edit:
			if (this->shell == nullptr)
			{
				this->shell = ref new Shell(this->app);
			}

			ui = this->shell;
			break;
		}
	}

	return ui;
}

void anim::MainPageVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::MainPageVM::AppPropertyChanged(const char *name)
{
	if (strcmp(name, "Mode") == 0)
	{
		this->shell = nullptr;
		this->NotifyPropertyChanged("Shell");
	}
}
