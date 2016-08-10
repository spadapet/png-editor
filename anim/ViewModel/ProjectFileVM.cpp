#include "pch.h"
#include "Core/Designer.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"
#include "ViewModel/ProjectFileVM.h"

anim::ProjectFileVM::ProjectFileVM(std::shared_ptr<ProjectFile> file)
	: file(file)
{
}

anim::ProjectFileVM::ProjectFileVM(Windows::Storage::StorageFile ^file)
	: ProjectFileVM(std::make_shared<ProjectFile>(file))
{
}

anim::ProjectFileVM::ProjectFileVM()
	: ProjectFileVM(std::shared_ptr<ProjectFile>())
{
	anim::AssertXamlDesigner();
}

anim::ProjectFileVM::~ProjectFileVM()
{
}

Windows::Storage::StorageFile ^anim::ProjectFileVM::File::get()
{
	return (this->file != nullptr) ? this->file->GetFile() : nullptr;
}

Platform::String ^anim::ProjectFileVM::DisplayName::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->DisplayName : "<null>";
}

Platform::String ^anim::ProjectFileVM::FullPath::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->Path : "<null>";
}

void anim::ProjectFileVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}
