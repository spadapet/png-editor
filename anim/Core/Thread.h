#pragma once

namespace anim
{
	bool IsMainThread();
	void RunOnMainThread(std::function<void()> func);
	void PostToMainThread(std::function<void()> func);
	Windows::UI::Core::CoreDispatcher ^GetMainDispatcher();
}
