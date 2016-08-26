#pragma once

#include "Core/Properties.h"
#include "View/Shell.g.h"
#include "View/Utility/BoolToObjectConverter.h"
#include "View/Utility/BoolToVisibleConverter.h"
#include "ViewModel/ShellVM.h"

namespace anim
{
	class AppState;

	public ref class Shell sealed
	{
	internal:
		Shell(std::shared_ptr<AppState> app);

	public:
		Shell();
		virtual ~Shell();

		property ShellVM ^State { ShellVM ^get(); }

	private:
		void OnDataTemplateUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
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
		Windows::Foundation::Point resizeStartPoint;
		Windows::UI::Core::CoreCursor ^resizeCursor;
		Windows::UI::Core::CoreCursor ^oldCursor;
	};
}
