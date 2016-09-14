#include "pch.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "Model/OpenImageFile.h"
#include "ViewModel/IOpenFileVM.h"
#include "ViewModel/OpenFileTabsVM.h"
#include "ViewModel/OpenImageVM.h"

anim::OpenFileTabsVM::OpenFileTabsVM(std::shared_ptr<AppState> app)
	: app(app)
	, files(ref new Platform::Collections::Vector<IOpenFileVM ^>())
{
	Platform::WeakReference weakOwner(this);

	this->fileOpenedCookie = this->app->FileOpened.Add([weakOwner](std::shared_ptr<OpenFile> file)
	{
		OpenFileTabsVM ^owner = weakOwner.Resolve<OpenFileTabsVM>();
		if (owner != nullptr)
		{
			owner->OnFileOpened(file);
		}
	});

	this->fileClosedCookie = this->app->FileClosed.Add([weakOwner](std::shared_ptr<OpenFile> file)
	{
		OpenFileTabsVM ^owner = weakOwner.Resolve<OpenFileTabsVM>();
		if (owner != nullptr)
		{
			owner->OnFileClosed(file);
		}
	});

	this->fileFocusCookie = this->app->FileFocus.Add([weakOwner](std::shared_ptr<OpenFile> file, bool &handled)
	{
		if (!handled)
		{
			OpenFileTabsVM ^owner = weakOwner.Resolve<OpenFileTabsVM>();
			if (owner != nullptr)
			{
				handled = owner->OnFileFocus(file);
			}
		}
	});

	this->ResetFiles();
}

anim::OpenFileTabsVM::OpenFileTabsVM()
	: OpenFileTabsVM(AppState::CreateForDesigner())
{
}

anim::OpenFileTabsVM::~OpenFileTabsVM()
{
	this->app->FileOpened.Remove(this->fileOpenedCookie);
	this->app->FileClosed.Remove(this->fileClosedCookie);
	this->app->FileFocus.Remove(this->fileFocusCookie);
}

Windows::Foundation::Collections::IVector<anim::IOpenFileVM ^> ^anim::OpenFileTabsVM::Files::get()
{
	return this->files;
}

anim::IOpenFileVM ^anim::OpenFileTabsVM::FocusFile::get()
{
	return this->focusFile;
}

void anim::OpenFileTabsVM::FocusFile::set(IOpenFileVM ^value)
{
	if (this->focusFile != value)
	{
		this->focusFile = value;
		this->NotifyPropertyChanged("FocusFile");
		this->NotifyPropertyChanged("FocusFileUserInterface");
	}
}

Windows::UI::Xaml::UIElement ^anim::OpenFileTabsVM::FocusFileUserInterface::get()
{
	return (this->focusFile != nullptr) ? this->focusFile->UserInterface : nullptr;
}

void anim::OpenFileTabsVM::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name ? name : ""));
}

void anim::OpenFileTabsVM::OnFileOpened(std::shared_ptr<OpenFile> file)
{
	std::shared_ptr<OpenImageFile> imageFile = std::dynamic_pointer_cast<OpenImageFile>(file);
	if (imageFile != nullptr)
	{
		OpenImageVM ^imageVM = ref new OpenImageVM(imageFile);
		this->files->Append(imageVM);
	}
}

void anim::OpenFileTabsVM::OnFileClosed(std::shared_ptr<OpenFile> file)
{
	std::shared_ptr<OpenImageFile> imageFile = std::dynamic_pointer_cast<OpenImageFile>(file);

	unsigned int i = 0;
	for (IOpenFileVM ^openFile : this->files)
	{
		if (imageFile != nullptr && openFile->AsImage != nullptr && openFile->AsImage->Model == imageFile)
		{
			if (this->focusFile == openFile && this->files->Size > 1)
			{
				this->FocusFile = this->files->GetAt((i == 0) ? 1 : 0);
			}

			openFile->Destroy();
			this->files->RemoveAt(i);
			break;
		}

		i++;
	}
}

bool anim::OpenFileTabsVM::OnFileFocus(std::shared_ptr<OpenFile> file)
{
	std::shared_ptr<OpenImageFile> imageFile = std::dynamic_pointer_cast<OpenImageFile>(file);

	for (IOpenFileVM ^openFile : this->files)
	{
		if (imageFile != nullptr && openFile->AsImage != nullptr && openFile->AsImage->Model == imageFile)
		{
			this->FocusFile = openFile;
			return true;
		}
	}

	return false;
}

void anim::OpenFileTabsVM::ResetFiles()
{
	for (IOpenFileVM ^openFile : this->files)
	{
		openFile->Destroy();
	}

	this->files->Clear();

	for (std::shared_ptr<OpenFile> file : this->app->GetOpenFiles())
	{
		this->OnFileOpened(file);
	}
}
