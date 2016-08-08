#include "pch.h"
#include "App.xaml.h"
#include "Model/AppState.h"
#include "ViewModel/PaneInfoVM.h"
#include "ViewModel/ShellVM.h"

anim::ShellVM::ShellVM(std::shared_ptr<AppState> app)
	: app(app)
	, appChangedCookie(NULL_EVENT_COOKIE)
	, panes(ref new Platform::Collections::Vector<PaneInfoVM ^>())
{
	Platform::WeakReference weakThis(this);

	this->ResetPanes();

	this->appChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<ShellVM>();
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
	if (value == nullptr)
	{
		value = this->nonePane;
	}

	if (this->activePane != value)
	{
		this->activePane->IsActive = false;
		this->activePane = value;
		this->activePane->IsActive = true;
		this->NotifyPropertyChanged("ActivePane");
		this->NotifyPropertyChanged("HasActivePane");
	}
}

bool anim::ShellVM::HasActivePane::get()
{
	return this->activePane != this->nonePane;
}

void anim::ShellVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ShellVM::AppPropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);

	if (allChanged || strcmp(name, "Panes"))
	{
		this->ResetPanes();
	}
}

void anim::ShellVM::ResetPanes()
{
	this->panes->Clear();
	this->nonePane = ref new PaneInfoVM(this->app, app->GetNonePane(), this);

	for (auto pane : this->app->GetPanes())
	{
		this->panes->Append(ref new PaneInfoVM(this->app, pane, this));
	}

	this->activePane = (this->panes->Size > 0)
		? this->panes->GetAt(0)
		: this->nonePane;
	this->activePane->IsActive = true;

	this->NotifyPropertyChanged("ActivePane");
	this->NotifyPropertyChanged("HasActivePane");
}
