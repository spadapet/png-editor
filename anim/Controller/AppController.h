#pragma once

namespace anim
{
	class AppState;
	class ProjectFolder;

	class AppController
	{
	public:
		AppController(std::shared_ptr<AppState> app);

		void AddProjectFolder();
		void RemoveProjectFolder(std::shared_ptr<ProjectFolder> folder);
		void OpenFile();

	private:
		std::shared_ptr<AppState> app;
	};
}
