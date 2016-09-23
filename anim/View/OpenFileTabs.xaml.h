#pragma once

#include "View/OpenFileTabs.g.h"
#include "View/Utility/OpenFileListBox.h"
#include "View/Utility/OpenFileListItem.h"
#include "View/Utility/OpenFileUiTemplateSelector.h"
#include "ViewModel/IOpenFileVM.h"
#include "ViewModel/OpenFileTabsVM.h"
#include "ViewModel/OpenImageVM.h"
#include "ViewModel/OpenNullFileVM.h"

namespace anim
{
	class AppState;
	ref class OpenFileTabsVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenFileTabs sealed
	{
	internal:
		OpenFileTabs(std::shared_ptr<AppState> app);

	public:
		OpenFileTabs();
		virtual ~OpenFileTabs();

		property OpenFileTabsVM ^State { OpenFileTabsVM ^get(); }

	private:
		void OnDataTemplateUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnTabPointerEntered(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void OnTabPointerExited(Platform::Object ^sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void OnClickOpenFile(Windows::UI::Xaml::Documents::Hyperlink ^sender, Windows::UI::Xaml::Documents::HyperlinkClickEventArgs ^args);
		void OnClickDropDown(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnTabsPopupOpened(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnTabsPopupClosed(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnDropDownButtonKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args);

		OpenFileTabsVM ^state;
	};
}

