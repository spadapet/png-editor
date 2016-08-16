#include "pch.h"
#include "View/Utility/BoolToObjectConverter.h"

anim::BoolToObjectConverter::BoolToObjectConverter()
{
}

Platform::Object ^anim::BoolToObjectConverter::ObjectTrue::get()
{
	return this->objectTrue;
}

void anim::BoolToObjectConverter::ObjectTrue::set(Platform::Object ^value)
{
	this->objectTrue = value;
}

Platform::Object ^anim::BoolToObjectConverter::ObjectFalse::get()
{
	return this->objectFalse;
}

void anim::BoolToObjectConverter::ObjectFalse::set(Platform::Object ^value)
{
	this->objectFalse = value;
}

Platform::Object ^anim::BoolToObjectConverter::Convert(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter,
	Platform::String ^language)
{
	Platform::IBox<Platform::Boolean> ^boolValue = dynamic_cast<Platform::IBox<Platform::Boolean> ^>(value);
	if (boolValue != nullptr)
	{
		return boolValue->Value
			? this->objectTrue
			: this->objectFalse;
	}

	return nullptr;
}

Platform::Object ^anim::BoolToObjectConverter::ConvertBack(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter, Platform::String ^language)
{
	return nullptr;
}
