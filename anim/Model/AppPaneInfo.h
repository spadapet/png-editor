#pragma once

#include "Core/Event.h"

namespace anim
{
	enum class AppPaneType
	{
		None,
		Files,
		Color,
		Layers,
		View,
		Animation,
	};

	typedef std::function<Windows::UI::Xaml::UIElement ^(AppPaneType)> AppPaneUiFactory;

	class AppPaneInfo
	{
	public:
		AppPaneInfo(AppPaneType type, AppPaneUiFactory &&paneFactory);
		~AppPaneInfo();

		SimpleEvent Disposed;
		ChangedEvent PropertyChanged;

		AppPaneType GetType() const;

	private:
		AppPaneType type;
		AppPaneUiFactory paneFactory;
	};
}
