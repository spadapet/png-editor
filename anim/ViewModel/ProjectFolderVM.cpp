#include "pch.h"
#include "Core/Designer.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "Model/ProjectFolder.h"
#include "ViewModel/ProjectFolderVM.h"

anim::ProjectFolderVM::ProjectFolderVM(std::shared_ptr<ProjectFolder> folder)
	: folder(folder)
	, expanded(false)
{
}

anim::ProjectFolderVM::ProjectFolderVM()
	: ProjectFolderVM(std::make_shared<ProjectFolder>(Windows::Storage::ApplicationData::Current->TemporaryFolder, nullptr))
{
	anim::AssertXamlDesigner();
}

anim::ProjectFolderVM::~ProjectFolderVM()
{
}

Windows::Storage::StorageFolder ^anim::ProjectFolderVM::Folder::get()
{
	return this->folder->GetFolder();
}

Platform::String ^anim::ProjectFolderVM::DisplayName::get()
{
	return this->folder->GetFolder()->DisplayName;
}

Platform::String ^anim::ProjectFolderVM::FullPath::get()
{
	return this->folder->GetFolder()->Path;
}

bool anim::ProjectFolderVM::HasChildren::get()
{
	return this->folder->HasItems();
}

bool anim::ProjectFolderVM::ShowExpanded::get()
{
	return this->expanded;
}

void anim::ProjectFolderVM::ShowExpanded::set(bool value)
{
	if (this->expanded != value)
	{
		this->expanded = value;
		this->NotifyPropertyChanged("ShowExpanded");
	}

	if (this->expanded)
	{
		this->folder->GetItems();
	}
}

int anim::ProjectFolderVM::Level::get()
{
	return this->folder->GetLevel();
}

void anim::ProjectFolderVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
