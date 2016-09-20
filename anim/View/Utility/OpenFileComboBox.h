#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenFileComboBox sealed : Windows::UI::Xaml::Controls::ComboBox
	{
	public:
		OpenFileComboBox();

	private:
		void OnLoaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args);
		void OnPopupVerticalOffsetChanged(Windows::UI::Xaml::DependencyObject ^obj, Windows::UI::Xaml::DependencyProperty ^prop);
		void OnPopupHorizontalOffsetChanged(Windows::UI::Xaml::DependencyObject ^obj, Windows::UI::Xaml::DependencyProperty ^prop);

		bool loaded;
	};
}
