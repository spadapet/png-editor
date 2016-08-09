#include "pch.h"
#include "Core/Thread.h"

void anim::AssertMainThread()
{
	assert(anim::IsMainThread());
}

bool anim::IsMainThread()
{
	return anim::GetMainDispatcher()->HasThreadAccess;
}

void anim::RunOnMainThread(std::function<void()> func)
{
	if (anim::IsMainThread())
	{
		func();
	}
	else
	{
		anim::PostToMainThread(func);
	}
}

void anim::PostToMainThread(std::function<void()> func)
{
	anim::GetMainDispatcher()->RunAsync(
		Windows::UI::Core::CoreDispatcherPriority::Normal,
		ref new Windows::UI::Core::DispatchedHandler(func));
}

Windows::UI::Core::CoreDispatcher ^anim::GetMainDispatcher()
{
	return Windows::UI::Xaml::Window::Current->Dispatcher;
}
