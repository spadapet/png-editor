#pragma once

#include "UI/FilesPane.g.h"
#include "ViewModel/FilesPaneVM.h"

namespace anim
{
	class AppState;

	public ref class FilesPane sealed
	{
	internal:
		FilesPane(AppState *app);

	public:
		virtual ~FilesPane();

		property FilesPaneVM ^State { FilesPaneVM ^get(); }

	private:
		FilesPaneVM ^state;
	};
}
