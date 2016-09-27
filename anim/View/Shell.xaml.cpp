#include "pch.h"
#include "Core/Windows.h"
#include "Core/Xaml.h"
#include "Model/AppState.h"
#include "View/OpenFileTabs.xaml.h"
#include "View/Shell.xaml.h"

anim::Shell::Shell(std::shared_ptr<AppState> app)
	: state(ref new ShellVM(app))
	, resizing(false)
	, resizeHovering(false)
	, setResizeCursor(false)
	, resizeStartSize(0)
	, resizeCursor(ref new Windows::UI::Core::CoreCursor(Windows::UI::Core::CoreCursorType::SizeWestEast, 0))
	, openFiles(ref new OpenFileTabs(app))
{
	Windows::UI::Xaml::Controls::Grid::SetColumn(this->openFiles, 3);

	this->DataContext = this->state;
	this->InitializeComponent();
	this->Children->Append(this->openFiles);
}

anim::Shell::Shell()
	: Shell(AppState::CreateForDesigner())
{
}

anim::Shell::~Shell()
{
}

anim::ShellVM ^anim::Shell::State::get()
{
	return this->state;
}

anim::OpenFileTabs ^anim::Shell::ActiveOpenFiles::get()
{
	return this->openFiles;
}

anim::IOpenFileVM ^anim::Shell::ActiveOpenFile::get()
{
	return (this->ActiveOpenFiles != nullptr)
		? this->ActiveOpenFiles->State->FocusFileOrNull
		: nullptr;
}

void anim::Shell::OnDataTemplateUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	anim::DisconnectDataTemplateBindings(sender);
}

void anim::Shell::OnLoaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	Windows::UI::Core::CoreWindow ^window = Windows::UI::Xaml::Window::Current->CoreWindow;

	this->keyDownCookie = window->KeyDown +=
		ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(
			this, &anim::Shell::OnWindowKeyDown);

	this->keyUpCookie = window->KeyUp +=
		ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow ^, Windows::UI::Core::KeyEventArgs ^>(
			this, &anim::Shell::OnWindowKeyUp);
}

void anim::Shell::OnUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	Windows::UI::Core::CoreWindow ^window = Windows::UI::Xaml::Window::Current->CoreWindow;

	window->KeyDown -= this->keyDownCookie;
	window->KeyUp -= this->keyUpCookie;
}

void anim::Shell::OnWindowKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args)
{
	Windows::UI::Core::CoreWindow ^window = Windows::UI::Xaml::Window::Current->CoreWindow;
	bool ctrl = anim::IsKeyDown(window, Windows::System::VirtualKey::Control);
	bool shift = anim::IsKeyDown(window, Windows::System::VirtualKey::Shift);

	switch (args->VirtualKey)
	{
	case Windows::System::VirtualKey::F4:
		if (ctrl && this->ActiveOpenFile != nullptr)
		{
			this->state->GetApp()->CloseFile(anim::GetOpenFileModel(this->ActiveOpenFile));
			args->Handled = true;
		}
		break;

	case Windows::System::VirtualKey::Tab:
		if (ctrl && this->ActiveOpenFiles != nullptr)
		{
			this->ActiveOpenFiles->State->CycleTabs(shift);
			args->Handled = true;
		}
		break;
	}
}

void anim::Shell::OnWindowKeyUp(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args)
{
	switch (args->VirtualKey)
	{
	case Windows::System::VirtualKey::Control:
		if (this->ActiveOpenFiles != nullptr)
		{
			this->ActiveOpenFiles->State->StopCycleTabs();
		}
		break;
	}
}

void anim::Shell::ResizeBorder_PointerCanceled(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	this->ResizeBorder_PointerCaptureLost(sender, args);
}

void anim::Shell::ResizeBorder_PointerCaptureLost(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	if (this->resizing)
	{
		this->resizing = false;
		this->resizeStartSize = 0;
		this->resizeStartPoint = Windows::Foundation::Point(0, 0);
		this->StopResizeCursor();
	}
}

void anim::Shell::ResizeBorder_PointerEntered(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	this->resizeHovering = true;
	this->StartResizeCursor();
}

void anim::Shell::ResizeBorder_PointerExited(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	this->resizeHovering = false;
	this->StopResizeCursor();
}

void anim::Shell::ResizeBorder_PointerMoved(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	if (this->resizing)
	{
		double offset = args->GetCurrentPoint(this)->Position.X - this->resizeStartPoint.X;
		this->state->PaneWidth = this->resizeStartSize + offset;
	}
}

void anim::Shell::ResizeBorder_PointerPressed(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	if (!this->resizing && this->ResizeBorder->CapturePointer(args->Pointer))
	{
		this->resizing = true;
		this->resizeStartPoint = args->GetCurrentPoint(this)->Position;
		this->resizeStartSize = this->state->PaneWidth;
	}
}

void anim::Shell::ResizeBorder_PointerReleased(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	this->ResizeBorder->ReleasePointerCaptures();
}

void anim::Shell::StartResizeCursor()
{
	if (!this->setResizeCursor)
	{
		Windows::UI::Core::CoreWindow ^window = Windows::UI::Xaml::Window::Current->CoreWindow;

		this->setResizeCursor = true;
		this->oldCursor = window->PointerCursor;
		window->PointerCursor = this->resizeCursor;
	}
}

void anim::Shell::StopResizeCursor()
{
	if (this->setResizeCursor && !this->resizing && !this->resizeHovering)
	{
		Windows::UI::Core::CoreWindow ^window = Windows::UI::Xaml::Window::Current->CoreWindow;

		this->setResizeCursor = false;
		window->PointerCursor = this->oldCursor;
		this->oldCursor = nullptr;
	}
}
