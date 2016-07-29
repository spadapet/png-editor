#include "pch.h"
#include "Model/AppState.h"
#include "UI/AppShell.xaml.h"
#include "ViewModel/MainPageVM.h"

anim::MainPageVM::MainPageVM(AppState *app)
	: app(app)
	, appDestroyedCookie(NULL_EVENT_COOKIE)
	, appChangedCookie(NULL_EVENT_COOKIE)
{
	if (this->app == nullptr)
	{
		// Probably running in XAML desigher
		return;
	}

	Platform::WeakReference weakThis(this);

	this->appDestroyedCookie = this->app->Destroyed.Add([weakThis]()
	{
		auto owner = weakThis.Resolve<MainPageVM>();
		if (owner != nullptr)
		{
			owner->app = nullptr;
			owner->NotifyPropertyChanged();
		}
	});

	this->appChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<MainPageVM>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});
}

anim::MainPageVM::~MainPageVM()
{
	if (this->app != nullptr)
	{
		this->app->PropertyChanged.Remove(this->appChangedCookie);
		this->app->Destroyed.Remove(this->appDestroyedCookie);
	}
}

Windows::UI::Xaml::UIElement ^anim::MainPageVM::Shell::get()
{
	if (this->shell == nullptr && this->app != nullptr)
	{
		switch (this->app->GetMode())
		{
		case AppMode::None:
		case AppMode::Edit:
			this->shell = ref new AppShell(this->app);
			break;
		}
	}

	return this->shell;
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
