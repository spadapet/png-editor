#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenFileUiTemplateSelector sealed : Windows::UI::Xaml::Controls::DataTemplateSelector
	{
	public:
		OpenFileUiTemplateSelector();

		property Windows::UI::Xaml::DataTemplate ^ImageTemplate { Windows::UI::Xaml::DataTemplate ^get(); void set(Windows::UI::Xaml::DataTemplate ^value); }
		property Windows::UI::Xaml::DataTemplate ^UnknownTemplate { Windows::UI::Xaml::DataTemplate ^get(); void set(Windows::UI::Xaml::DataTemplate ^value); }
		property Windows::UI::Xaml::DataTemplate ^NullTemplate { Windows::UI::Xaml::DataTemplate ^get(); void set(Windows::UI::Xaml::DataTemplate ^value); }

	protected:
		virtual Windows::UI::Xaml::DataTemplate ^SelectTemplateCore(Platform::Object ^item) override;
		virtual Windows::UI::Xaml::DataTemplate ^SelectTemplateCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container) override;

	private:
		Windows::UI::Xaml::DataTemplate ^imageTemplate;
		Windows::UI::Xaml::DataTemplate ^unknownTemplate;
		Windows::UI::Xaml::DataTemplate ^nullTemplate;
	};
}
