#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public interface class IOpenFileVM
	{
		void Destroy();

		property Platform::String ^Name { Platform::String ^get(); }
		property Platform::String ^Tooltip { Platform::String ^get(); }
		property bool IsDirty { bool get(); }
		property Windows::UI::Xaml::UIElement ^UserInterface { Windows::UI::Xaml::UIElement ^get(); }
		property Windows::UI::Xaml::Input::ICommand ^CloseCommand { Windows::UI::Xaml::Input::ICommand ^get(); }
	};
}
