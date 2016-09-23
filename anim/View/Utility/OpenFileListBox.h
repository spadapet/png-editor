#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenFileListBox sealed : Windows::UI::Xaml::Controls::ListBox
	{
	public:
		OpenFileListBox();

		void SetPopup(Windows::UI::Xaml::Controls::Primitives::Popup ^popup);

	protected:
		virtual Windows::UI::Xaml::DependencyObject ^GetContainerForItemOverride() override;

	private:
		Platform::WeakReference popup;
	};
}
