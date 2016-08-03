#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Properties sealed
	{
	public:
		static property Windows::UI::Xaml::DependencyProperty ^VisualStateProperty { Windows::UI::Xaml::DependencyProperty ^get(); }

		static Platform::String ^GetVisualState(Windows::UI::Xaml::UIElement ^element);
		static void SetVisualState(Windows::UI::Xaml::UIElement ^element, Platform::String ^value);
		static void OnVisualStateChanged(Windows::UI::Xaml::DependencyObject ^sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs ^args);
	};
}
