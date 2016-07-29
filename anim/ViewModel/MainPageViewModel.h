#pragma once

namespace anim
{
	class AppState;
	ref class AppShell;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MainPageViewModel sealed
	{
	internal:
		MainPageViewModel(AppState *app);

	public:
		virtual ~MainPageViewModel();

		property Windows::UI::Xaml::UIElement ^Shell { Windows::UI::Xaml::UIElement ^get(); }

	private:
		AppShell ^shell;
	};
}
