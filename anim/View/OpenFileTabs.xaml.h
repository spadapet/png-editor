#pragma once

#include "View/OpenFileTabs.g.h"
#include "ViewModel/IOpenFileVM.h"
#include "ViewModel/OpenFileTabsVM.h"

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
		OpenFileTabsVM ^state;
	};
}
