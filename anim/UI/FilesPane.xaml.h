#pragma once

#include "UI/FilesPane.g.h"

namespace anim
{
	class AppState;
	ref class FilesPaneVM;

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
