#include "pch.h"
#include "App.xaml.h"
#include "Model/AppState.h"
#include "ViewModel/AppShellState.h"
#include "ViewModel/ProjectFolderViewModel.h"

anim::AppShellState::AppShellState()
	: parent(&App::Current->GetGlobalState())
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
	, projectFolderAddedCookie(NULL_EVENT_COOKIE)
	, projectFolderRemovedCookie(NULL_EVENT_COOKIE)
	, projectFolders(ref new Platform::Collections::Vector<ProjectFolderViewModel ^>())
{
	assert(parent != nullptr);

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

anim::AppShellState::~AppShellState()
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

Windows::Foundation::Collections::IVector<anim::ProjectFolderViewModel ^> ^anim::AppShellState::ProjectFolders::get()
{
	return this->projectFolders;
}

void anim::AppShellState::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::AppShellState::ModelPropertyChanged(const char *name)
{
}
