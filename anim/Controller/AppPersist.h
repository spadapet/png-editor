#pragma once

namespace anim
{
	class AppState;

	concurrency::task<std::shared_ptr<AppState>> LoadAppState();
	concurrency::task<void> SaveAppState(std::shared_ptr<AppState> app);
}
