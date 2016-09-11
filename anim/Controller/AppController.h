#pragma once

namespace anim
{
	class AppState;

	class AppController
	{
	public:
		AppController(std::shared_ptr<AppState> app);

		void AddProjectFolder();
		void OpenFile();

	private:
		std::shared_ptr<AppState> app;
	};
}
