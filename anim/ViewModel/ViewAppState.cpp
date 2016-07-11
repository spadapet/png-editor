#include "pch.h"
#include "Model/AppState.h"
#include "ViewModel/ViewAppState.h"
#include "ViewModel/ViewProjectFolder.h"

anim::ViewAppState::ViewAppState(AppState *parent)
	: parent(parent)
	, parentDisposedCookie(NULL_EVENT_COOKIE)
	, parentChangedCookie(NULL_EVENT_COOKIE)
	, projectFolderAddedCookie(NULL_EVENT_COOKIE)
	, projectFolderRemovedCookie(NULL_EVENT_COOKIE)
	, projectFolders(ref new Platform::Collections::Vector<ViewProjectFolder ^>())
{
	assert(parent != nullptr);

	for (Windows::Storage::StorageFolder ^folder : parent->GetProjectFolders())
	{
		this->projectFolders->Append(ref new ViewProjectFolder(folder));
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
		this->projectFolders->Append(ref new ViewProjectFolder(folder));
	});

	this->projectFolderRemovedCookie = this->parent->ProjectFolderRemoved.Add([this](Windows::Storage::StorageFolder ^folder)
	{
		for (unsigned int i = 0; i < this->projectFolders->Size; i++)
		{
			ViewProjectFolder ^projectFolder = this->projectFolders->GetAt(i);
			if (projectFolder->Folder == folder)
			{
				projectFolder->Dispose();
				this->projectFolders->RemoveAt(i);
				break;
			}
		}
	});
}

anim::ViewAppState::~ViewAppState()
{
	for (ViewProjectFolder ^projectFolder : this->projectFolders)
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

Windows::Foundation::Collections::IVector<anim::ViewProjectFolder ^> ^anim::ViewAppState::ProjectFolders::get()
{
	return this->projectFolders;
}

void anim::ViewAppState::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ViewAppState::ModelPropertyChanged(const char *name)
{
}
