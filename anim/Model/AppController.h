#pragma once

namespace anim
{
	class AppState;

	class AppController
	{
	public:
		AppController(std::shared_ptr<AppState> app);
		~AppController();

		void AddFolder();
		void OpenFile();

	private:
		std::shared_ptr<AppState> app;
	};
}
