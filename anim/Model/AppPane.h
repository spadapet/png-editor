#pragma once

#include "Core/Event.h"

namespace anim
{
	enum class AppPaneType
	{
		None,
		Files,
	};

	typedef std::function<Windows::UI::Xaml::UIElement ^(AppPaneType)> AppPaneUiFactory;

	class AppPane
	{
	public:
		AppPane(AppPaneType type, AppPaneUiFactory &&paneFactory);
		~AppPane();

		SimpleEvent Disposed;
		ChangedEvent PropertyChanged;

		AppPaneType GetType() const;

	private:
		AppPaneType type;
		AppPaneUiFactory paneFactory;
	};
}
