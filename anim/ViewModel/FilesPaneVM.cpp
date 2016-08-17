#include "pch.h"
#include "App.xaml.h"
#include "Core/Command.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/FilesPaneVM.h"

anim::FilesPaneVM::FilesPaneVM(std::shared_ptr<AppState> app)
	: app(app)
	, controller(app)
	, appChangedCookie(NULL_EVENT_COOKIE)
	, projectFolderAddedCookie(NULL_EVENT_COOKIE)
	, projectFolderRemovedCookie(NULL_EVENT_COOKIE)
	, projectFolders(ref new Platform::Collections::Vector<ProjectFolderVM ^>())
{
	Platform::WeakReference weakThis(this);

	this->ResetProjectFolders();

	this->appChangedCookie = this->app->PropertyChanged.Add([weakThis](const char *name)
	{
		FilesPaneVM ^owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			owner->AppPropertyChanged(name);
		}
	});

	this->projectFolderAddedCookie = this->app->ProjectFolderAdded.Add([weakThis](std::shared_ptr<ProjectFolder> folder)
	{
		FilesPaneVM ^owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			owner->projectFolders->Append(ref new ProjectFolderVM(folder));
			owner->NotifyPropertyChanged("HasProjectFolders");
		}
	});

	this->projectFolderRemovedCookie = this->app->ProjectFolderRemoved.Add([weakThis](std::shared_ptr<ProjectFolder> folder)
	{
		FilesPaneVM ^owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			for (unsigned int i = 0; i < owner->projectFolders->Size; i++)
			{
				ProjectFolderVM ^projectFolder = owner->projectFolders->GetAt(i);
				if (projectFolder->Folder == folder->GetFolder())
				{
					owner->projectFolders->RemoveAt(i);
					owner->NotifyPropertyChanged("HasProjectFolders");
					break;
				}
			}
		}
	});

	this->addFolderCommand = ref new Command([weakThis](Platform::Object ^)
	{
		FilesPaneVM ^owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			owner->controller.AddProjectFolder();
		}
	});

	this->openFileCommand = ref new Command([weakThis](Platform::Object ^)
	{
		FilesPaneVM ^owner = weakThis.Resolve<FilesPaneVM>();
		if (owner != nullptr)
		{
			owner->controller.OpenFile();
		}
	});
}

anim::FilesPaneVM::FilesPaneVM()
	: FilesPaneVM(AppState::CreateForDesigner())
{
}

anim::FilesPaneVM::~FilesPaneVM()
{
	this->app->PropertyChanged.Remove(this->appChangedCookie);
	this->app->ProjectFolderAdded.Remove(this->projectFolderAddedCookie);
	this->app->ProjectFolderRemoved.Remove(this->projectFolderRemovedCookie);
}

Windows::Foundation::Collections::IVector<anim::ProjectFolderVM ^> ^anim::FilesPaneVM::ProjectFolders::get()
{
	return this->projectFolders;
}

bool anim::FilesPaneVM::HasProjectFolders::get()
{
	return this->projectFolders->Size > 0;
}

anim::TreeRow ^anim::FilesPaneVM::SelectedTreeRow::get()
{
	return nullptr;
}

void anim::FilesPaneVM::SelectedTreeRow::set(TreeRow ^value)
{
}

Windows::UI::Xaml::Input::ICommand ^anim::FilesPaneVM::AddFolderCommand::get()
{
	return this->addFolderCommand;
}

Windows::UI::Xaml::Input::ICommand ^anim::FilesPaneVM::OpenFileCommand::get()
{
	return this->openFileCommand;
}

void anim::FilesPaneVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::FilesPaneVM::AppPropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);

	if (allChanged || strcmp(name, "ProjectFolders") == 0)
	{
		this->ResetProjectFolders();
	}
}

void anim::FilesPaneVM::ResetProjectFolders()
{
	this->projectFolders->Clear();

	for (std::shared_ptr<ProjectFolder> folder : app->GetProjectFolders())
	{
		this->projectFolders->Append(ref new ProjectFolderVM(folder));
	}

	this->NotifyPropertyChanged("HasProjectFolders");
}
