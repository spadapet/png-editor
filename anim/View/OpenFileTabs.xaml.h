#pragma once

#include "View/OpenFileTabs.g.h"
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
		void TabItem_PointerEntered(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);
		void TabItem_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args);

		OpenFileTabsVM ^state;
	};
}
