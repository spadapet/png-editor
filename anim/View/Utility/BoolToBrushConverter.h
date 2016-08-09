#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class BoolToBrushConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		BoolToBrushConverter();

		property Windows::UI::Xaml::Media::Brush ^BrushTrue
		{
			Windows::UI::Xaml::Media::Brush ^get();
			void set(Windows::UI::Xaml::Media::Brush ^value);
		}

		property Windows::UI::Xaml::Media::Brush ^BrushFalse
		{
			Windows::UI::Xaml::Media::Brush ^get();
			void set(Windows::UI::Xaml::Media::Brush ^value);
		}

		// IValueConverter
		virtual Platform::Object ^Convert(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);
		virtual Platform::Object ^ConvertBack(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);

	private:
		Windows::UI::Xaml::Media::Brush ^brushTrue;
		Windows::UI::Xaml::Media::Brush ^brushFalse;
	};
}
