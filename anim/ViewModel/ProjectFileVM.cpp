#include "pch.h"
#include "Core/Designer.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "ViewModel/ProjectFileVM.h"

anim::ProjectFileVM::ProjectFileVM(std::shared_ptr<ProjectFile> file)
	: file(file)
{
}

anim::ProjectFileVM::ProjectFileVM()
	: ProjectFileVM(nullptr)
{
	anim::AssertXamlDesigner();
}

anim::ProjectFileVM::~ProjectFileVM()
{
}

Windows::Storage::StorageFile ^anim::ProjectFileVM::File::get()
{
	return this->file->GetFile();
}

Platform::String ^anim::ProjectFileVM::DisplayName::get()
{
	return this->file->GetFile()->DisplayName;
}

Platform::String ^anim::ProjectFileVM::FullPath::get()
{
	return this->file->GetFile()->Path;
}

void anim::ProjectFileVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
