#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class LevelToIndentConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		LevelToIndentConverter();

		property double IndentSize { double get(); void set(double value); }

		// IValueConverter
		virtual Platform::Object ^Convert(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);
		virtual Platform::Object ^ConvertBack(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);

	private:
		double indentSize;
	};
}
