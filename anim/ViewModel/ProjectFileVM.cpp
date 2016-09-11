#include "pch.h"
#include "Controller/AppController.h"
#include "Core/Command.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "ViewModel/ProjectFileVM.h"

static Windows::UI::Xaml::Input::ICommand ^activateCommand = nullptr;

anim::ProjectFileVM::ProjectFileVM(std::shared_ptr<ProjectFile> file, ProjectFolderVM ^parent)
	: file(file)
	, weakParent(parent != nullptr ? Platform::WeakReference(parent) : Platform::WeakReference(nullptr))
{
}

anim::ProjectFileVM::ProjectFileVM()
	: ProjectFileVM(std::shared_ptr<ProjectFile>(), nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFileVM::~ProjectFileVM()
{
}

void anim::ProjectFileVM::Destroy()
{
	this->file = nullptr;
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
	return (this->file != nullptr) ? this->file->GetFile()->Name : "<null>";
}

Platform::String ^anim::ProjectFileVM::FullPath::get()
{
	return (this->file != nullptr) ? this->file->GetItem()->Path : "<null>";
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
			if (file != nullptr)
			{
				std::shared_ptr<AppState> app = file->Model->GetAppState();
				app->OpenFile(file->Model);
			}
		});
	}

	return ::activateCommand;
}

Windows::UI::Xaml::Input::ICommand ^anim::ProjectFileVM::DeleteCommand::get()
{
	return nullptr;
}

Windows::Storage::StorageFile ^anim::ProjectFileVM::File::get()
{
	return (this->file != nullptr) ? this->file->GetFile() : nullptr;
}

bool anim::ProjectFileVM::IsOpen::get()
{
	return false;
}

void anim::ProjectFileVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
