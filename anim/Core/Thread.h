#pragma once

namespace anim
{
	void SetMainThread();
	void AssertMainThread();
	bool IsMainThread();
	void RunOnMainThread(std::function<void()> func);
	void PostToMainThread(std::function<void()> func);
	Windows::UI::Core::CoreDispatcher ^GetMainDispatcher();
}
