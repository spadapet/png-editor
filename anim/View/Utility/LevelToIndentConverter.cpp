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
		double indent = intValue->Value * this->IndentSize;

		if (Windows::UI::Xaml::Interop::TypeName(double::typeid).Name == targetType.Name)
		{
			return indent;
		}
		else if (Windows::UI::Xaml::Interop::TypeName(Windows::UI::Xaml::GridLength::typeid).Name == targetType.Name)
		{
			return Windows::UI::Xaml::GridLength(indent);
		}
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
