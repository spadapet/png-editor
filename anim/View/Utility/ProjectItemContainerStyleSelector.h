#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectItemContainerStyleSelector sealed : Windows::UI::Xaml::Controls::StyleSelector
	{
	public:
		ProjectItemContainerStyleSelector();

		property Windows::UI::Xaml::Style ^RootFolderStyle { Windows::UI::Xaml::Style ^get(); void set(Windows::UI::Xaml::Style ^value); }
		property Windows::UI::Xaml::Style ^FolderStyle { Windows::UI::Xaml::Style ^get(); void set(Windows::UI::Xaml::Style ^value); }
		property Windows::UI::Xaml::Style ^FileStyle { Windows::UI::Xaml::Style ^get(); void set(Windows::UI::Xaml::Style ^value); }
		property Windows::UI::Xaml::Style ^ItemStyle { Windows::UI::Xaml::Style ^get(); void set(Windows::UI::Xaml::Style ^value); }

	protected:
		virtual Windows::UI::Xaml::Style ^SelectStyleCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container) override;

	private:
		Windows::UI::Xaml::Style ^rootFolderStyle;
		Windows::UI::Xaml::Style ^folderStyle;
		Windows::UI::Xaml::Style ^fileStyle;
		Windows::UI::Xaml::Style ^itemStyle;
	};
}
