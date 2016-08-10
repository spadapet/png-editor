#include "pch.h"
#include "Core/Thread.h"

static Windows::UI::Core::CoreDispatcher ^mainDispatcher;

void anim::SetMainThread()
{
	::mainDispatcher = Windows::UI::Xaml::Window::Current->Dispatcher;
}

void anim::AssertMainThread()
{
	assert(anim::IsMainThread());
}

bool anim::IsMainThread()
{
	return ::mainDispatcher != nullptr && ::mainDispatcher->HasThreadAccess;
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
	assert(::mainDispatcher != nullptr);
	return ::mainDispatcher;
}
