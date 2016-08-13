#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ProjectItemTemplateSelector sealed : Windows::UI::Xaml::Controls::DataTemplateSelector
	{
	public:
		ProjectItemTemplateSelector();

		virtual Windows::UI::Xaml::DataTemplate ^SelectTemplateCore(Platform::Object ^item) override;
		virtual Windows::UI::Xaml::DataTemplate ^SelectTemplateCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container) override;

		property Windows::UI::Xaml::DataTemplate ^RootFolderTemplate { Windows::UI::Xaml::DataTemplate ^get(); void set(Windows::UI::Xaml::DataTemplate ^value); }
		property Windows::UI::Xaml::DataTemplate ^FolderTemplate { Windows::UI::Xaml::DataTemplate ^get(); void set(Windows::UI::Xaml::DataTemplate ^value); }
		property Windows::UI::Xaml::DataTemplate ^FileTemplate { Windows::UI::Xaml::DataTemplate ^get(); void set(Windows::UI::Xaml::DataTemplate ^value); }
		property Windows::UI::Xaml::DataTemplate ^ItemTemplate { Windows::UI::Xaml::DataTemplate ^get(); void set(Windows::UI::Xaml::DataTemplate ^value); }

	private:
		Windows::UI::Xaml::DataTemplate ^rootFolderTemplate;
		Windows::UI::Xaml::DataTemplate ^folderTemplate;
		Windows::UI::Xaml::DataTemplate ^fileTemplate;
		Windows::UI::Xaml::DataTemplate ^itemTemplate;
	};
}
