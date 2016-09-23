#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenFileListItem sealed : Windows::UI::Xaml::Controls::ListBoxItem
	{
	internal:
		OpenFileListItem(Windows::UI::Xaml::Controls::Primitives::Popup ^popup);

	public:
		OpenFileListItem();

		void SetPopup(Windows::UI::Xaml::Controls::Primitives::Popup ^popup);

	protected:
		virtual void OnKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args) override;
		virtual void OnPointerReleased(Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args) override;

	private:
		void ClosePopup();

		Platform::WeakReference popup;
	};
}
