#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class BoolToObjectConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		BoolToObjectConverter();

		property Platform::Object ^ObjectTrue { Platform::Object ^get(); void set(Platform::Object ^value); }
		property Platform::Object ^ObjectFalse { Platform::Object ^get(); void set(Platform::Object ^value); }

		// IValueConverter
		virtual Platform::Object ^Convert(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);
		virtual Platform::Object ^ConvertBack(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);

	private:
		Platform::Object ^objectTrue;
		Platform::Object ^objectFalse;
	};
}
