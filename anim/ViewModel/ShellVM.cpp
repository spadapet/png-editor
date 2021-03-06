#include "pch.h"
#include "App.xaml.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "ViewModel/ShellVM.h"

anim::ShellVM::ShellVM(std::shared_ptr<AppState> app)
	: app(app)
	, appChangedCookie(NULL_EVENT_COOKIE)
	, panes(ref new Platform::Collections::Vector<PaneInfoVM ^>())
	, paneWidth(200)
{
	Platform::WeakReference weakThis(this);

	this->ResetPanes();

	this->appChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		ShellVM ^owner = weakThis.Resolve<ShellVM>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});
}

anim::ShellVM::ShellVM()
	: ShellVM(AppState::CreateForDesigner())
{
}

anim::ShellVM::~ShellVM()
{
	this->app->PropertyChanged.Remove(this->appChangedCookie);
}

std::shared_ptr<anim::AppState> anim::ShellVM::GetApp() const
{
	return this->app;
}

Windows::Foundation::Collections::IVector<anim::PaneInfoVM ^> ^anim::ShellVM::Panes::get()
{
	return this->panes;
}

anim::PaneInfoVM ^anim::ShellVM::ActivePane::get()
{
	return this->activePane;
}

void anim::ShellVM::ActivePane::set(PaneInfoVM ^value)
{
	if (value == nullptr || !value->IsVisible)
	{
		value = this->nonePane;
	}

	if (this->activePane != value)
	{
		this->activePane->PropertyChanged -= this->paneChangedCookie;
		this->activePane->IsActive = false;

		this->activePane = value;

		this->activePane->IsActive = true;
		this->paneChangedCookie = this->activePane->PropertyChanged +=
			ref new Windows::UI::Xaml::Data::PropertyChangedEventHandler(this, &ShellVM::PanePropertyChanged);

		this->NotifyPropertyChanged("ActivePane");
		this->NotifyPropertyChanged("HasActivePane");
	}
}

bool anim::ShellVM::HasActivePane::get()
{
	return this->activePane != this->nonePane;
}

double anim::ShellVM::PaneWidth::get()
{
	return this->paneWidth;
}

void anim::ShellVM::PaneWidth::set(double value)
{
	value = std::floor(value / 4) * 4;

	if (value < 64)
	{
		value = 64;
	}

	if (value > 512)
	{
		value = 512;
	}

	if (this->paneWidth != value)
	{
		this->paneWidth = value;
		this->NotifyPropertyChanged("PaneWidth");
	}
}

void anim::ShellVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ShellVM::AppPropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);

	if (allChanged || strcmp(name, "Panes") == 0)
	{
		this->ResetPanes();
	}
}

void anim::ShellVM::PanePropertyChanged(Platform::Object ^sender, Windows::UI::Xaml::Data::PropertyChangedEventArgs ^args)
{
	bool allChanged = args->PropertyName == nullptr || args->PropertyName->Length() == 0;

	if (allChanged || args->PropertyName == "IsActive" || args->PropertyName == "IsVisible")
	{
		if (this->activePane->IsActive && !this->activePane->IsVisible)
		{
			Platform::WeakReference weakOwner(this);

			anim::PostToMainThread([weakOwner]()
			{
				ShellVM ^owner = weakOwner.Resolve<ShellVM>();
				if (owner != nullptr)
				{
					PaneInfoVM ^foundPane = nullptr;

					for (PaneInfoVM ^pane : owner->Panes)
					{
						if (pane->IsVisible)
						{
							foundPane = pane;
							break;
						}
					}

					owner->ActivePane = foundPane;
				}
			});
		}
	}
}

void anim::ShellVM::ResetPanes()
{
	if (this->activePane != nullptr)
	{
		this->activePane->PropertyChanged -= this->paneChangedCookie;
	}

	this->panes->Clear();
	this->nonePane = ref new PaneInfoVM(this->app, app->GetNonePane(), this);
	this->activePane = this->nonePane;

	for (const std::shared_ptr<PaneInfo> &pane : this->app->GetPanes())
	{
		PaneInfoVM ^paneVM = ref new PaneInfoVM(this->app, pane, this);
		this->panes->Append(paneVM);

		if (this->activePane == this->nonePane && paneVM->IsVisible)
		{
			this->activePane = paneVM;
		}
	}

	this->activePane->IsActive = true;
	this->paneChangedCookie = this->activePane->PropertyChanged +=
		ref new Windows::UI::Xaml::Data::PropertyChangedEventHandler(this, &ShellVM::PanePropertyChanged);

	this->NotifyPropertyChanged("ActivePane");
	this->NotifyPropertyChanged("HasActivePane");
}
