#include "pch.h"
#include "Core/Command.h"
#include "Model/AppState.h"
#include "Model/OpenImageFile.h"
#include "Model/ProjectFile.h"
#include "ViewModel/OpenImageVM.h"

static Windows::UI::Xaml::Input::ICommand ^closeCommand = nullptr;

anim::OpenImageVM::OpenImageVM(std::shared_ptr<OpenImageFile> file)
	: file(file)
{
	Platform::WeakReference weakOwner(this);

	this->fileChangedCookie = this->file->PropertyChanged.Add([weakOwner](const char *name)
	{
		OpenImageVM ^owner = weakOwner.Resolve<OpenImageVM>();
		if (owner != nullptr)
		{
			owner->FilePropertyChanged(name);
		}
	});
}

anim::OpenImageVM::OpenImageVM()
{
}

anim::OpenImageVM::~OpenImageVM()
{
	if (this->file != nullptr)
	{
		this->file->PropertyChanged.Remove(this->fileChangedCookie);
	}
}

void anim::OpenImageVM::Destroy()
{
}

std::shared_ptr<anim::OpenImageFile> anim::OpenImageVM::Model::get()
{
	return this->file;
}

Platform::String ^anim::OpenImageVM::Name::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->GetFile()->Name : "<null>";
}

Platform::String ^anim::OpenImageVM::Tooltip::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->GetFile()->Path : "<null>";
}

bool anim::OpenImageVM::IsDirty::get()
{
	return this->file != nullptr && this->file->IsDirty();
}

Windows::UI::Xaml::UIElement ^anim::OpenImageVM::UserInterface::get()
{
	return nullptr;
}

Windows::UI::Xaml::Input::ICommand ^anim::OpenImageVM::CloseCommand::get()
{
	if (::closeCommand == nullptr)
	{
		::closeCommand = ref new anim::Command([](Platform::Object ^item)
		{
			OpenImageVM ^file = dynamic_cast<OpenImageVM ^>(item);
			if (file != nullptr && file->Model != nullptr)
			{
				std::shared_ptr<AppState> app = file->Model->GetFile()->GetAppState();
				app->CloseFile(file->Model);
			}
		});
	}

	return ::closeCommand;
}

void anim::OpenImageVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::OpenImageVM::FilePropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr) || name[0] == 0;

	if (allChanged || strcmp(name, "IsDirty") == 0)
	{
		this->NotifyPropertyChanged("IsDirty");
	}
}
