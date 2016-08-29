#pragma once

#include "View/FilesPane.g.h"
#include "View/Utility/FlatProjectItem.h"
#include "View/Utility/FlatProjectList.h"
#include "View/Utility/LevelToIndentConverter.h"
#include "View/Utility/ProjectItemContainerStyleSelector.h"
#include "View/Utility/ProjectItemTemplateSelector.h"
#include "ViewModel/FilesPaneVM.h"

namespace anim
{
	class AppState;

	public ref class FilesPane sealed
	{
	internal:
		FilesPane(std::shared_ptr<AppState> app);

	public:
		FilesPane();
		virtual ~FilesPane();

		property FilesPaneVM ^State { FilesPaneVM ^get(); }

	private:
		void OnClickAddFolderLink(Windows::UI::Xaml::Documents::Hyperlink ^sender, Windows::UI::Xaml::Documents::HyperlinkClickEventArgs ^args);
		void OnDataTemplateUnloaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);

		FilesPaneVM ^state;
	};
}
