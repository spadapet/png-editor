#include "pch.h"
#include "UI/Utility/BoolToVisibleConverter.h"

anim::BoolToVisibleConverter::BoolToVisibleConverter()
	: inverse(false)
{
}

bool anim::BoolToVisibleConverter::Inverse::get()
{
	return this->inverse;
}

void anim::BoolToVisibleConverter::Inverse::set(bool value)
{
	this->inverse = value;
}

Platform::Object ^anim::BoolToVisibleConverter::Convert(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter,
	Platform::String ^language)
{
	Platform::IBox<Platform::Boolean> ^boolValue = dynamic_cast<Platform::IBox<Platform::Boolean> ^>(value);
	if (boolValue != nullptr)
	{
		return (this->inverse ? !boolValue->Value : boolValue->Value)
			? Windows::UI::Xaml::Visibility::Visible
			: Windows::UI::Xaml::Visibility::Collapsed;
	}

	return Windows::UI::Xaml::Visibility::Collapsed;
}

Platform::Object ^anim::BoolToVisibleConverter::ConvertBack(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter, Platform::String ^language)
{
	return nullptr;
}
