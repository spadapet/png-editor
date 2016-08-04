#include "pch.h"
#include "UI/Utility/BoolToBrushConverter.h"

anim::BoolToBrushConverter::BoolToBrushConverter()
{
}

Windows::UI::Xaml::Media::Brush ^anim::BoolToBrushConverter::BrushTrue::get()
{
	return this->brushTrue;
}

void anim::BoolToBrushConverter::BrushTrue::set(Windows::UI::Xaml::Media::Brush ^value)
{
	this->brushTrue = value;
}

Windows::UI::Xaml::Media::Brush ^anim::BoolToBrushConverter::BrushFalse::get()
{
	return this->brushFalse;
}

void anim::BoolToBrushConverter::BrushFalse::set(Windows::UI::Xaml::Media::Brush ^value)
{
	this->brushFalse = value;
}

Platform::Object ^anim::BoolToBrushConverter::Convert(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter,
	Platform::String ^language)
{
	Platform::IBox<Platform::Boolean> ^boolValue = dynamic_cast<Platform::IBox<Platform::Boolean> ^>(value);
	if (boolValue != nullptr)
	{
		return boolValue->Value
			? this->brushTrue
			: this->brushFalse;
	}

	return nullptr;
}

Platform::Object ^anim::BoolToBrushConverter::ConvertBack(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter, Platform::String ^language)
{
	return nullptr;
}
