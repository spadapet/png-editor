#pragma once

namespace anim
{
	ref class FlatProjectItem;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FlatProjectList sealed : Windows::UI::Xaml::Controls::ListBox
	{
	public:
		FlatProjectList();
		virtual ~FlatProjectList();

		FlatProjectItem ^GetParent(FlatProjectItem ^item);
		FlatProjectItem ^GetChild(FlatProjectItem ^item);

	protected:
		virtual Windows::UI::Xaml::DependencyObject ^GetContainerForItemOverride() override;
	};
}
