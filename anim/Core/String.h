#pragma once

namespace anim
{
	std::string ConvertString(Platform::String ^str);
	Platform::String ^ConvertString(const std::string &str);
	Platform::String ^ConvertString(const char *str, size_t len = INVALID_SIZE);

	namespace Resource
	{
		Platform::String ^GetString(Platform::String ^id);
	}
}
