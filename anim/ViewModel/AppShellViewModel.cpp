#include "pch.h"
#include "App.xaml.h"
#include "Model/AppState.h"
#include "ViewModel/AppPaneViewModel.h"
#include "ViewModel/AppShellViewModel.h"
#include "ViewModel/ProjectFolderViewModel.h"

anim::AppShellViewModel::AppShellViewModel()
	: parent(&App::Current->GetGlobalState())
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
	, projectFolderAddedCookie(NULL_EVENT_COOKIE)
	, projectFolderRemovedCookie(NULL_EVENT_COOKIE)
	, panes(ref new Platform::Collections::Vector<AppPaneViewModel ^>())
	, projectFolders(ref new Platform::Collections::Vector<ProjectFolderViewModel ^>())
{
	for (auto &pane : parent->GetPanes())
	{
		this->panes->Append(ref new AppPaneViewModel(pane.get()));
	}

	for (Windows::Storage::StorageFolder ^folder : parent->GetProjectFolders())
	{
		this->projectFolders->Append(ref new ProjectFolderViewModel(folder));
	}

	this->parentDisposedCookie = this->parent->Disposed.Add([this]()
	{
		this->parent = nullptr;
	});

	this->parentChangedCookie = this->parent->PropertyChanged.Add([this](const char *name)
	{
		this->ModelPropertyChanged(name);
	});

	this->projectFolderAddedCookie = this->parent->ProjectFolderAdded.Add([this](Windows::Storage::StorageFolder ^folder)
	{
		this->projectFolders->Append(ref new ProjectFolderViewModel(folder));
	});

	this->projectFolderRemovedCookie = this->parent->ProjectFolderRemoved.Add([this](Windows::Storage::StorageFolder ^folder)
	{
		for (unsigned int i = 0; i < this->projectFolders->Size; i++)
		{
			ProjectFolderViewModel ^projectFolder = this->projectFolders->GetAt(i);
			if (projectFolder->Folder == folder)
			{
				projectFolder->Dispose();
				this->projectFolders->RemoveAt(i);
				break;
			}
		}
	});
}

anim::AppShellViewModel::~AppShellViewModel()
{
	for (ProjectFolderViewModel ^projectFolder : this->projectFolders)
	{
		projectFolder->Dispose();
	}

	if (this->parent != nullptr)
	{
		this->parent->PropertyChanged.Remove(this->parentChangedCookie);
		this->parent->Disposed.Remove(this->parentDisposedCookie);
		this->parent->ProjectFolderAdded.Remove(this->projectFolderAddedCookie);
		this->parent->ProjectFolderRemoved.Remove(this->projectFolderRemovedCookie);
	}
}

Windows::Foundation::Collections::IVector<anim::AppPaneViewModel ^> ^anim::AppShellViewModel::Panes::get()
{
	return this->panes;
}

Windows::Foundation::Collections::IVector<anim::ProjectFolderViewModel ^> ^anim::AppShellViewModel::ProjectFolders::get()
{
	return this->projectFolders;
}

void anim::AppShellViewModel::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::AppShellViewModel::ModelPropertyChanged(const char *name)
{
}
