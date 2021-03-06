#pragma once

#include "Core/Event.h"

namespace anim
{
	ref class PaneInfoVM;

	enum class PaneType
	{
		None,
		Files,
		Color,
		Layers,
		View,
		Animation,
	};

	typedef std::function<Windows::UI::Xaml::UIElement ^(PaneType)> PaneUiFactory;

	class PaneInfo
	{
	public:
		PaneInfo(PaneType type, PaneUiFactory &&paneFactory);
		~PaneInfo();

		ChangedEvent PropertyChanged;

		PaneType GetType() const;
		Windows::UI::Xaml::UIElement ^CreatePane() const;

	private:
		PaneType type;
		PaneUiFactory paneFactory;
	};
}
