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

		// ListBoxItem
		virtual void OnKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args) override;

		property FlatProjectList ^List { FlatProjectList ^get(); }
		property IProjectItemVM ^Item { IProjectItemVM ^get(); }

	private:
		Platform::WeakReference weakList;
	};
}
