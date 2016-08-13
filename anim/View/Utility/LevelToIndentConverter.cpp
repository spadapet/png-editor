#include "pch.h"
#include "View/Utility/LevelToIndentConverter.h"

anim::LevelToIndentConverter::LevelToIndentConverter()
	: indentSize(0)
{
}

double anim::LevelToIndentConverter::IndentSize::get()
{
	return this->indentSize;
}

void anim::LevelToIndentConverter::IndentSize::set(double value)
{
	this->indentSize = value;
}

Platform::Object ^anim::LevelToIndentConverter::Convert(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter,
	Platform::String ^language)
{
	Platform::IBox<int> ^intValue = dynamic_cast<Platform::IBox<int> ^>(value);
	if (intValue != nullptr)
	{
		return intValue->Value * this->IndentSize;
	}

	return nullptr;
}

Platform::Object ^anim::LevelToIndentConverter::ConvertBack(
	Platform::Object ^value,
	Windows::UI::Xaml::Interop::TypeName targetType,
	Platform::Object ^parameter,
	Platform::String ^language)
{
	return nullptr;
}
