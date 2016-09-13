#include "pch.h"
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
				owner->OnFileFocus(file);
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
			openFile->Destroy();
			this->files->RemoveAt(i);
		}
		else
		{
			i++;
		}
	}
}

bool anim::OpenFileTabsVM::OnFileFocus(std::shared_ptr<OpenFile> file)
{
	return false;
}

void anim::OpenFileTabsVM::ResetFiles()
{
	this->files->Clear();

	for (std::shared_ptr<OpenFile> file : this->app->GetOpenFiles())
	{
		this->OnFileOpened(file);
	}
}
