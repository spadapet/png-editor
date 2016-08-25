#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FlatProjectList sealed : Windows::UI::Xaml::Controls::ListBox
	{
	public:
		FlatProjectList();
		virtual ~FlatProjectList();

	protected:
		virtual Windows::UI::Xaml::DependencyObject ^GetContainerForItemOverride() override;
	};
}
