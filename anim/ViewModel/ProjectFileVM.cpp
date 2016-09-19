#include "pch.h"
#include "Controller/AppController.h"
#include "Core/Command.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "ViewModel/ProjectFileVM.h"

static Windows::UI::Xaml::Input::ICommand ^activateCommand = nullptr;
static Windows::UI::Xaml::Input::ICommand ^closeCommand = nullptr;

anim::ProjectFileVM::ProjectFileVM(std::shared_ptr<ProjectFile> file, ProjectFolderVM ^parent)
	: file(file)
	, weakParent(parent != nullptr ? Platform::WeakReference(parent) : Platform::WeakReference(nullptr))
{
	Platform::WeakReference weakOwner(this);

	this->fileChangedCookie = this->file->PropertyChanged.Add([weakOwner](const char *name)
	{
		ProjectFileVM ^owner = weakOwner.Resolve<ProjectFileVM>();
		if (owner != nullptr)
		{
			owner->FilePropertyChanged(name);
		}
	});
}

anim::ProjectFileVM::ProjectFileVM()
	: ProjectFileVM(std::shared_ptr<ProjectFile>(), nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFileVM::~ProjectFileVM()
{
	if (this->file != nullptr)
	{
		this->file->PropertyChanged.Remove(this->fileChangedCookie);
	}
}

void anim::ProjectFileVM::Destroy()
{
	if (this->file != nullptr)
	{
		this->file->PropertyChanged.Remove(this->fileChangedCookie);
		this->file = nullptr;
	}

	this->weakParent = nullptr;
	this->NotifyPropertyChanged();
}

std::shared_ptr<anim::ProjectFile> anim::ProjectFileVM::Model::get()
{
	return this->file;
}

Windows::Storage::IStorageItem ^anim::ProjectFileVM::Item::get()
{
	return (this->file != nullptr) ? this->file->GetItem() : nullptr;
}

Platform::String ^anim::ProjectFileVM::DisplayName::get()
{
	return (this->file != nullptr) ? this->file->GetName() : "<null>";
}

Platform::String ^anim::ProjectFileVM::FullPath::get()
{
	return (this->file != nullptr) ? this->file->GetPath() : "<null>";
}

int anim::ProjectFileVM::Level::get()
{
	IProjectItemVM ^parent = this->Parent;
	return (parent != nullptr) ? parent->Level + 1 : 0;
}

anim::ProjectFileVM ^anim::ProjectFileVM::AsFile::get()
{
	return this;
}

anim::ProjectFolderVM ^anim::ProjectFileVM::AsFolder::get()
{
	return nullptr;
}

anim::IProjectItemVM ^anim::ProjectFileVM::Parent::get()
{
	return this->weakParent.Resolve<IProjectItemVM>();
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectFileVM::ActivateCommand::get()
{
	if (::activateCommand == nullptr)
	{
		::activateCommand = ref new anim::Command([](Platform::Object ^item)
		{
			ProjectFileVM ^file = dynamic_cast<ProjectFileVM ^>(item);
			if (file != nullptr && file->Model != nullptr)
			{
				std::shared_ptr<AppState> app = file->Model->GetAppState();
				app->EditFile(file->Model);
			}
		});
	}

	return ::activateCommand;
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectFileVM::DeleteCommand::get()
{
	return nullptr;
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectFileVM::CloseCommand::get()
{
	if (::closeCommand == nullptr)
	{
		::closeCommand = ref new anim::Command([](Platform::Object ^item)
		{
			ProjectFileVM ^file = dynamic_cast<ProjectFileVM ^>(item);
			if (file != nullptr && file->Model != nullptr && file->Model->GetOpenFile() != nullptr)
			{
				std::shared_ptr<AppState> app = file->Model->GetAppState();
				app->CloseFile(file->Model->GetOpenFile());
			}
		},
		[](Platform::Object ^item)
		{
			ProjectFileVM ^file = dynamic_cast<ProjectFileVM ^>(item);
			return file != nullptr && file->Model != nullptr && file->Model->GetOpenFile() != nullptr;
		});
	}

	return ::closeCommand;
}

Windows::Storage::StorageFile ^anim::ProjectFileVM::File::get()
{
	return (this->file != nullptr) ? this->file->GetFile() : nullptr;
}

bool anim::ProjectFileVM::IsOpen::get()
{
	return this->file != nullptr && this->file->IsOpen();
}

void anim::ProjectFileVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::ProjectFileVM::FilePropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);

	if (allChanged || strcmp(name, "IsOpen") == 0)
	{
		this->NotifyPropertyChanged("IsOpen");
	}
}
