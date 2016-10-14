#include "pch.h"
#include "Core/Command.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "Model/OpenImageFile.h"
#include "Model/ProjectFile.h"
#include "View/ImageEditor.xaml.h"
#include "ViewModel/ImageVM.h"
#include "ViewModel/OpenImageVM.h"

static Windows::UI::Xaml::Input::ICommand ^closeCommand = nullptr;

anim::OpenImageVM::OpenImageVM(std::shared_ptr<OpenImageFile> file)
	: file(file)
	, active(false)
	, hover(false)
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

	this->ResetImage();
}

anim::OpenImageVM::OpenImageVM()
{
	anim::AssertXamlDesigner();
}

anim::OpenImageVM::~OpenImageVM()
{
	this->Destroy();
}

void anim::OpenImageVM::Destroy()
{
	this->active = false;

	if (this->file != nullptr)
	{
		this->file->PropertyChanged.Remove(this->fileChangedCookie);
		this->file = nullptr;
	}

	if (this->imageEditor != nullptr)
	{
		this->imageEditor->Destroy();
		this->imageEditor = nullptr;
	}

	if (this->image != nullptr)
	{
		this->image->Destroy();
		this->image = nullptr;
	}

	this->NotifyPropertyChanged();
}

anim::ImageVM ^anim::OpenImageVM::Image::get()
{
	return this->image;
}

Windows::Storage::StorageFile ^anim::OpenImageVM::File::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->GetFile() : nullptr;
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
	return this->Path;
}

Platform::String ^anim::OpenImageVM::Path::get()
{
	return (this->file != nullptr) ? this->file->GetFile()->GetFile()->Path : "<null>";
}

bool anim::OpenImageVM::IsDirty::get()
{
	return this->file != nullptr && this->file->IsDirty();
}

bool anim::OpenImageVM::IsActive::get()
{
	return this->active;
}

void anim::OpenImageVM::IsActive::set(bool value)
{
	if (this->active != value)
	{
		this->active = value;

		if (this->active && this->file != nullptr)
		{
			this->file->Initialize();
		}

		this->NotifyPropertyChanged("IsActive");
		this->NotifyPropertyChanged("ShowCloseButton");
	}
}

bool anim::OpenImageVM::TabMouseHover::get()
{
	return this->hover;
}

void anim::OpenImageVM::TabMouseHover::set(bool value)
{
	if (this->hover != value)
	{
		this->hover = value;
		this->NotifyPropertyChanged("TabMouseHover");
		this->NotifyPropertyChanged("ShowCloseButton");
	}
}

bool anim::OpenImageVM::ShowCloseButton::get()
{
	return this->IsActive || this->TabMouseHover;
}

Windows::UI::Xaml::UIElement ^anim::OpenImageVM::UserInterface::get()
{
	if (this->imageEditor == nullptr && this->image != nullptr)
	{
		this->imageEditor = ref new ImageEditor(this->image);
	}

	return imageEditor;
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

anim::OpenImageVM ^anim::OpenImageVM::AsImage::get()
{
	return this;
}

void anim::OpenImageVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::OpenImageVM::FilePropertyChanged(const char *name)
{
	bool allChanged = (name == nullptr || *name == 0);

	if (allChanged || strcmp(name, "IsDirty") == 0)
	{
		this->NotifyPropertyChanged("IsDirty");
	}

	if (allChanged || strcmp(name, "Image") == 0)
	{
		this->ResetImage();
	}
}

void anim::OpenImageVM::ResetImage()
{
	this->image = nullptr;
	this->imageEditor = nullptr;

	if (this->file->GetImage() != nullptr)
	{
		this->image = ref new ImageVM(this->file->GetImage(), this->active);
	}

	this->NotifyPropertyChanged("Image");
	this->NotifyPropertyChanged("UserInterface");
}
