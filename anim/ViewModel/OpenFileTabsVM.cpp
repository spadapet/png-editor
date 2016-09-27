#include "pch.h"
#include "Core/Thread.h"
#include "Model/AppState.h"
#include "Model/OpenImageFile.h"
#include "Model/ProjectFile.h"
#include "ViewModel/IOpenFileVM.h"
#include "ViewModel/OpenFileTabsVM.h"
#include "ViewModel/OpenImageVM.h"
#include "ViewModel/OpenNullFileVM.h"

anim::OpenFileTabsVM::OpenFileTabsVM(std::shared_ptr<AppState> app)
	: app(app)
	, files(ref new Platform::Collections::Vector<IOpenFileVM ^>())
	, nullFile(ref new OpenNullFileVM())
{
	this->currentTabOrder = this->tabOrder.end();
	this->focusFile = this->nullFile;
	this->focusFile->IsActive = true;

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
				handled = owner->OnFileFocus(file, false);
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
	this->focusFile->IsActive = false;

	this->app->FileOpened.Remove(this->fileOpenedCookie);
	this->app->FileClosed.Remove(this->fileClosedCookie);
	this->app->FileFocus.Remove(this->fileFocusCookie);
}

void anim::OpenFileTabsVM::CycleTabs(bool reverse)
{
	if (this->currentTabOrder != this->tabOrder.end())
	{
		if (reverse)
		{
			if (this->currentTabOrder == this->tabOrder.begin())
			{
				this->currentTabOrder = this->tabOrder.end();
			}

			this->currentTabOrder--;
		}
		else
		{
			this->currentTabOrder++;

			if (this->currentTabOrder == this->tabOrder.end())
			{
				this->currentTabOrder = this->tabOrder.begin();
			}
		}

		this->OnFileFocus(*this->currentTabOrder, true);
	}
}

void anim::OpenFileTabsVM::StopCycleTabs()
{
	if (this->currentTabOrder != this->tabOrder.begin() &&
		this->currentTabOrder != this->tabOrder.end())
	{
		IOpenFileVM ^file = *this->currentTabOrder;
		this->tabOrder.erase(this->currentTabOrder);
		this->tabOrder.push_front(file);
		this->currentTabOrder = this->tabOrder.begin();
	}
}

std::shared_ptr<anim::AppState> anim::OpenFileTabsVM::GetApp() const
{
	return this->app;
}

Windows::Foundation::Collections::IVector<anim::IOpenFileVM ^> ^anim::OpenFileTabsVM::Files::get()
{
	return this->files;
}

bool anim::OpenFileTabsVM::HasFiles::get()
{
	return this->files->Size > 0;
}

anim::IOpenFileVM ^anim::OpenFileTabsVM::FocusFile::get()
{
	return this->focusFile;
}

void anim::OpenFileTabsVM::FocusFile::set(IOpenFileVM ^value)
{
	if (value == nullptr)
	{
		value = this->nullFile;
	}

	if (this->focusFile != value)
	{
		this->focusFile->IsActive = false;
		this->focusFile = value;
		this->focusFile->IsActive = true;

		this->NotifyPropertyChanged("FocusFile");
		this->NotifyPropertyChanged("FocusFileOrNull");
	}
}

anim::IOpenFileVM ^anim::OpenFileTabsVM::FocusFileOrNull::get()
{
	return (this->focusFile != this->nullFile) ? this->focusFile : nullptr;
}

void anim::OpenFileTabsVM::FocusFileOrNull::set(IOpenFileVM ^value)
{
	if (value != nullptr && this->focusFile != value)
	{
		this->OnFileFocus(value, false);
	}
}

Windows::UI::Xaml::Controls::ListBox ^anim::OpenFileTabsVM::TabsList::get()
{
	return this->tabsList.Resolve<Windows::UI::Xaml::Controls::ListBox>();
}

void anim::OpenFileTabsVM::TabsList::set(Windows::UI::Xaml::Controls::ListBox ^value)
{
	this->tabsList = value;
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
		this->tabOrder.push_front(imageVM);
		this->currentTabOrder = this->tabOrder.begin();

		if (this->files->Size == 1)
		{
			this->NotifyPropertyChanged("HasFiles");
		}

		if (this->focusFile == this->nullFile)
		{
			this->OnFileFocus(file, false);
		}
	}
}

void anim::OpenFileTabsVM::OnFileClosed(std::shared_ptr<OpenFile> file)
{
	unsigned int i = 0;
	for (IOpenFileVM ^openFile : this->files)
	{
		if (file->GetFile()->Equals(openFile->File))
		{
			if (this->focusFile == openFile)
			{
				this->FocusFile = (this->files->Size > 1)
					? this->files->GetAt((i == 0) ? 1 : 0)
					: this->nullFile;
			}

			openFile->Destroy();
			this->files->RemoveAt(i);

			auto tabIter = std::find(this->tabOrder.begin(), this->tabOrder.end(), openFile);
			bool currentTab = (this->currentTabOrder == tabIter);
			this->tabOrder.erase(tabIter);

			if (currentTab)
			{
				this->currentTabOrder = this->tabOrder.begin();
			}

			if (this->files->Size == 0)
			{
				this->NotifyPropertyChanged("HasFiles");
			}

			break;
		}

		i++;
	}
}

bool anim::OpenFileTabsVM::OnFileFocus(std::shared_ptr<OpenFile> file, bool tabCycle)
{
	bool success = false;

	for (IOpenFileVM ^openFile : this->files)
	{
		if (file->GetFile()->Equals(openFile->File))
		{
			success = this->OnFileFocus(openFile, tabCycle);
			break;
		}
	}

	return success;
}

bool anim::OpenFileTabsVM::OnFileFocus(IOpenFileVM ^file, bool tabCycle)
{
	auto iter = std::find(this->tabOrder.begin(), this->tabOrder.end(), file);
	if (iter == this->tabOrder.end())
	{
		assert(false);
		return false;
	}

	this->FocusFile = file;
	this->currentTabOrder = iter;

	if (!tabCycle)
	{
		this->StopCycleTabs();
	}

	Windows::UI::Xaml::Controls::ListBox ^list = this->TabsList;
	if (list != nullptr)
	{
		anim::PostToMainThread([list, file]()
		{
			if (list->SelectedItem == file)
			{
				list->UpdateLayout();
				list->ScrollIntoView(file);
			}
		});
	}

	return true;
}

void anim::OpenFileTabsVM::ResetFiles()
{
	this->FocusFile = this->nullFile;

	for (IOpenFileVM ^openFile : this->files)
	{
		openFile->Destroy();
	}

	this->files->Clear();

	for (std::shared_ptr<OpenFile> file : this->app->GetOpenFiles())
	{
		this->OnFileOpened(file);
	}

	this->NotifyPropertyChanged();
}
