#include "pch.h"
#include "App.xaml.h"
#include "Model/AppState.h"
#include "ViewModel/ProjectFolderVM.h"
#include "ViewModel/FilesPaneVM.h"

anim::FilesPaneVM::FilesPaneVM(std::shared_ptr<AppState> app)
	: app(app)
	, appChangedCookie(NULL_EVENT_COOKIE)
	, projectFolderAddedCookie(NULL_EVENT_COOKIE)
	, projectFolderRemovedCookie(NULL_EVENT_COOKIE)
	, projectFolders(ref new Platform::Collections::Vector<ProjectFolderVM ^>())
{
	Platform::WeakReference weakThis(this);

	for (Windows::Storage::StorageFolder ^folder : app->GetProjectFolders())
	{
		this->projectFolders->Append(ref new ProjectFolderVM(folder));
	}

	this->appChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		auto owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});

	this->projectFolderAddedCookie = this->app->ProjectFolderAdded.Add([weakThis](Windows::Storage::StorageFolder ^folder)
	{
		auto owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			owner->projectFolders->Append(ref new ProjectFolderVM(folder));
		}
	});

	this->projectFolderRemovedCookie = this->app->ProjectFolderRemoved.Add([weakThis](Windows::Storage::StorageFolder ^folder)
	{
		auto owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			for (unsigned int i = 0; i < owner->projectFolders->Size; i++)
			{
				ProjectFolderVM ^projectFolder = owner->projectFolders->GetAt(i);
				if (projectFolder->Folder == folder)
				{
					owner->projectFolders->RemoveAt(i);
					break;
				}
			}
		}
	});
}

anim::FilesPaneVM::FilesPaneVM()
	: FilesPaneVM(AppState::CreateForDesigner())
{
}

anim::FilesPaneVM::~FilesPaneVM()
{
	if (this->app != nullptr)
	{
		this->app->PropertyChanged.Remove(this->appChangedCookie);
		this->app->ProjectFolderAdded.Remove(this->projectFolderAddedCookie);
		this->app->ProjectFolderRemoved.Remove(this->projectFolderRemovedCookie);
	}
}

Windows::Foundation::Collections::IVector<anim::ProjectFolderVM ^> ^anim::FilesPaneVM::ProjectFolders::get()
{
	return this->projectFolders;
}

void anim::FilesPaneVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::FilesPaneVM::AppPropertyChanged(const char *name)
{
}
