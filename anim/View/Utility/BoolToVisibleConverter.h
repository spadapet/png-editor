#pragma once

namespace anim
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class BoolToVisibleConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		BoolToVisibleConverter();

		property bool Inverse { bool get(); void set(bool value); }

		// IValueConverter
		virtual Platform::Object ^Convert(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);
		virtual Platform::Object ^ConvertBack(Platform::Object ^value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^parameter, Platform::String ^language);

	private:
		bool inverse;
	};
}
