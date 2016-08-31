#pragma once

namespace anim
{
	class AppState;
	class ProjectFile;
	class ProjectFolder;

	class AppController
	{
	public:
		AppController(std::shared_ptr<AppState> app);

		void AddProjectFolder();
		void RemoveProjectFolder(std::shared_ptr<ProjectFolder> folder);
		void OpenFile();
		void OpenFile(std::shared_ptr<ProjectFile> file);

	private:
		std::shared_ptr<AppState> app;
	};
}
