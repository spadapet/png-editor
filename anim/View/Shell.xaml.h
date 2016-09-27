#pragma once

#include "Core/Properties.h"
#include "View/Shell.g.h"
#include "View/Utility/BoolToObjectConverter.h"
#include "View/Utility/BoolToVisibleConverter.h"
#include "ViewModel/ShellVM.h"

namespace anim
{
	class AppState;
	interface class IOpenFileVM;
	ref class OpenFileTabs;

	public ref class Shell sealed
	{
	internal:
		Shell(std::shared_ptr<AppState> app);

	public:
		Shell();
		virtual ~Shell();

		property ShellVM ^State { ShellVM ^get(); }
		property OpenFileTabs ^ActiveOpenFiles { OpenFileTabs ^get(); }
		property IOpenFileVM ^ActiveOpenFile { IOpenFileVM ^get(); }

	private:
		void OnDataTemplateUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnLoaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnWindowKeyDown(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);
		void OnWindowKeyUp(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);

		void ResizeBorder_PointerCanceled(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void ResizeBorder_PointerCaptureLost(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void ResizeBorder_PointerEntered(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void ResizeBorder_PointerExited(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void ResizeBorder_PointerMoved(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void ResizeBorder_PointerPressed(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void ResizeBorder_PointerReleased(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void StartResizeCursor();
		void StopResizeCursor();

		ShellVM ^state;
		bool resizing;
		bool resizeHovering;
		bool setResizeCursor;
		double resizeStartSize;
		Windows::Foundation::EventRegistrationToken keyDownCookie;
		Windows::Foundation::EventRegistrationToken keyUpCookie;
		Windows::Foundation::Point resizeStartPoint;
		Windows::UI::Core::CoreCursor ^resizeCursor;
		Windows::UI::Core::CoreCursor ^oldCursor;
		OpenFileTabs ^openFiles;
	};
}
