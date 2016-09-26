#include "pch.h"
#include "Core/Windows.h"

bool anim::IsKeyDown(Windows::UI::Core::CoreWindow ^window, Windows::System::VirtualKey key)
{
	return (window->GetKeyState(key) & Windows::UI::Core::CoreVirtualKeyStates::Down) ==
		Windows::UI::Core::CoreVirtualKeyStates::Down;
}