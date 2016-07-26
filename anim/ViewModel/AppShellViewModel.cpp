#include "pch.h"
#include "App.xaml.h"
#include "Model/AppState.h"
#include "ViewModel/AppPaneInfoViewModel.h"
#include "ViewModel/AppShellViewModel.h"
#include "ViewModel/ProjectFolderViewModel.h"

anim::AppShellViewModel::AppShellViewModel()
	: parent(&App::Current->GetGlobalState())
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
	, projectFolderAddedCookie(NULL_EVENT_COOKIE)
	, projectFolderRemovedCookie(NULL_EVENT_COOKIE)
	, panes(ref new Platform::Collections::Vector<AppPaneInfoViewModel ^>())
	, projectFolders(ref new Platform::Collections::Vector<ProjectFolderViewModel ^>())
{
	Platform::WeakReference weakThis(this);

	this->nonePane = ref new AppPaneInfoViewModel(&parent->GetNonePane(), this);

	for (auto &pane : parent->GetPanes())
	{
		this->panes->Append(ref new AppPaneInfoViewModel(pane.get(), this));
	}

	this->activePane = (this->panes->Size > 0)
		? this->panes->GetAt(0)
		: this->nonePane;
	this->activePane->IsActive = true;

	for (Windows::Storage::StorageFolder ^folder : parent->GetProjectFolders())
	{
		this->projectFolders->Append(ref new ProjectFolderViewModel(folder));
	}

	this->parentDisposedCookie = this->parent->Disposed.Add([weakThis]()
	{
		auto owner = weakThis.Resolve<AppShellViewModel>();
		if (owner != nullptr)
		{
			owner->parent = nullptr;
			owner->NotifyPropertyChanged();
		}
	});

	this->parentChangedCookie = this->parent->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<AppShellViewModel>();
		if (owner != nullptr)
		{
			owner->ModelPropertyChanged(name);
		}
	});

	this->projectFolderAddedCookie = this->parent->ProjectFolderAdded.Add([weakThis](Windows::Storage::StorageFolder ^folder)
	{
		auto owner = weakThis.Resolve<AppShellViewModel>();
		if (owner != nullptr)
		{
			owner->projectFolders->Append(ref new ProjectFolderViewModel(folder));
		}
	});

	this->projectFolderRemovedCookie = this->parent->ProjectFolderRemoved.Add([weakThis](Windows::Storage::StorageFolder ^folder)
	{
		auto owner = weakThis.Resolve<AppShellViewModel>();
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

anim::AppShellViewModel::~AppShellViewModel()
{
	for (ProjectFolderViewModel ^projectFolder : this->projectFolders)
	{
		projectFolder->Destroy();
	}

	if (this->parent != nullptr)
	{
		this->parent->PropertyChanged.Remove(this->parentChangedCookie);
		this->parent->Disposed.Remove(this->parentDisposedCookie);
		this->parent->ProjectFolderAdded.Remove(this->projectFolderAddedCookie);
		this->parent->ProjectFolderRemoved.Remove(this->projectFolderRemovedCookie);
	}
}

Windows::Foundation::Collections::IVector<anim::AppPaneInfoViewModel ^> ^anim::AppShellViewModel::Panes::get()
{
	return this->panes;
}

Windows::Foundation::Collections::IVector<anim::ProjectFolderViewModel ^> ^anim::AppShellViewModel::ProjectFolders::get()
{
	return this->projectFolders;
}

anim::AppPaneInfoViewModel ^anim::AppShellViewModel::ActivePane::get()
{
	return this->activePane;
}

void anim::AppShellViewModel::ActivePane::set(AppPaneInfoViewModel ^value)
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

void anim::AppShellViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::AppShellViewModel::ModelPropertyChanged(const char *name)
{
}
