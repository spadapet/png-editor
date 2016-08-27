#pragma once

namespace anim
{
	ref class FlatProjectList;
	interface class IProjectItemVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FlatProjectItem sealed : Windows::UI::Xaml::Controls::ListBoxItem
	{
	public:
		FlatProjectItem();
		FlatProjectItem(FlatProjectList ^list);
		virtual ~FlatProjectItem();

		property FlatProjectList ^List { FlatProjectList ^get(); }
		property IProjectItemVM ^Item { IProjectItemVM ^get(); }

	protected:
		// ListBoxItem
		virtual void OnKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args) override;
		virtual void OnPointerPressed(Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args) override;
		virtual void OnTapped(Windows::UI::Xaml::Input::TappedRoutedEventArgs ^args) override;
		virtual void OnDoubleTapped(Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs ^args) override;

	private:
		Platform::WeakReference weakList;
	};
}
