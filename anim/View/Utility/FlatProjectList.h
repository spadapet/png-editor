#pragma once

namespace anim
{
	ref class FlatProjectItem;
	interface class IProjectItemVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FlatProjectList sealed : Windows::UI::Xaml::Controls::ListBox
	{
	public:
		FlatProjectList();
		virtual ~FlatProjectList();

		void SelectSingle(IProjectItemVM ^item);

	protected:
		virtual Windows::UI::Xaml::DependencyObject ^GetContainerForItemOverride() override;
	};
}
