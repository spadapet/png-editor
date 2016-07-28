#include "pch.h"
#include "App.xaml.h"
#include "Model/AppState.h"
#include "ViewModel/PaneInfoViewModel.h"
#include "ViewModel/ProjectFolderViewModel.h"
#include "ViewModel/ShellViewModel.h"

anim::ShellViewModel::ShellViewModel(AppState *app)
	: app(app)
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
	, projectFolderAddedCookie(NULL_EVENT_COOKIE)
	, projectFolderRemovedCookie(NULL_EVENT_COOKIE)
	, panes(ref new Platform::Collections::Vector<PaneInfoViewModel ^>())
	, projectFolders(ref new Platform::Collections::Vector<ProjectFolderViewModel ^>())
{
	if (this->app == nullptr)
	{
		// Probably running in XAML desigher
		return;
	}

	Platform::WeakReference weakThis(this);

	this->nonePane = ref new PaneInfoViewModel(this->app, &app->GetNonePane(), this);

	for (auto &pane : this->app->GetPanes())
	{
		this->panes->Append(ref new PaneInfoViewModel(this->app, pane.get(), this));
	}

	this->activePane = (this->panes->Size > 0)
		? this->panes->GetAt(0)
		: this->nonePane;
	this->activePane->IsActive = true;

	for (Windows::Storage::StorageFolder ^folder : app->GetProjectFolders())
	{
		this->projectFolders->Append(ref new ProjectFolderViewModel(folder));
	}

	this->parentDisposedCookie = this->app->Destroyed.Add([weakThis]()
	{
		auto owner = weakThis.Resolve<ShellViewModel>();
		if (owner != nullptr)
		{
			owner->app = nullptr;
			owner->NotifyPropertyChanged();
		}
	});

	this->parentChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<ShellViewModel>();
		if (owner != nullptr)
		{
			owner->ModelPropertyChanged(name);
		}
	});

	this->projectFolderAddedCookie = this->app->ProjectFolderAdded.Add([weakThis](Windows::Storage::StorageFolder ^folder)
	{
		auto owner = weakThis.Resolve<ShellViewModel>();
		if (owner != nullptr)
		{
			owner->projectFolders->Append(ref new ProjectFolderViewModel(folder));
		}
	});

	this->projectFolderRemovedCookie = this->app->ProjectFolderRemoved.Add([weakThis](Windows::Storage::StorageFolder ^folder)
	{
		auto owner = weakThis.Resolve<ShellViewModel>();
		if (owner != nullptr)
		{
			for (unsigned int i = 0; i < owner->projectFolders->Size; i++)
			{
				ProjectFolderViewModel ^projectFolder = owner->projectFolders->GetAt(i);
				if (projectFolder->Folder == folder)
				{
					projectFolder->Destroy();
					owner->projectFolders->RemoveAt(i);
					break;
				}
			}
		}
	});
}

anim::ShellViewModel::~ShellViewModel()
{
	for (ProjectFolderViewModel ^projectFolder : this->projectFolders)
	{
		projectFolder->Destroy();
	}

	if (this->app != nullptr)
	{
		this->app->PropertyChanged.Remove(this->parentChangedCookie);
		this->app->Destroyed.Remove(this->parentDisposedCookie);
		this->app->ProjectFolderAdded.Remove(this->projectFolderAddedCookie);
		this->app->ProjectFolderRemoved.Remove(this->projectFolderRemovedCookie);
	}
}

Windows::Foundation::Collections::IVector<anim::PaneInfoViewModel ^> ^anim::ShellViewModel::Panes::get()
{
	return this->panes;
}

Windows::Foundation::Collections::IVector<anim::ProjectFolderViewModel ^> ^anim::ShellViewModel::ProjectFolders::get()
{
	return this->projectFolders;
}

anim::PaneInfoViewModel ^anim::ShellViewModel::ActivePane::get()
{
	return this->activePane;
}

void anim::ShellViewModel::ActivePane::set(PaneInfoViewModel ^value)
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
	}
}

void anim::ShellViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ShellViewModel::ModelPropertyChanged(const char *name)
{
}
