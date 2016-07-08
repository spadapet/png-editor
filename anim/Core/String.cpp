#include "pch.h"
#include "Core/String.h"

std::string anim::ConvertString(Platform::String ^str)
{
	if (str == nullptr || str->Length() == 0)
	{
		return std::string();
	}

	size_t newLen = ::WideCharToMultiByte(CP_UTF8, 0, str->Data(), str->Length(), nullptr, 0, nullptr, nullptr);

	std::string buf;
	buf.resize(newLen);
	::WideCharToMultiByte(CP_UTF8, 0, str->Data(), str->Length(), &buf[0], (int)newLen, nullptr, nullptr);

	return buf;
}

Platform::String ^anim::ConvertString(const std::string &str)
{
	return anim::ConvertString(str.c_str(), str.size());
}

Platform::String ^ anim::ConvertString(const char *str, size_t len)
{
	if (len == INVALID_SIZE)
	{
		len = str != nullptr ? strlen(str) : 0;
	}

	if (str == nullptr || *str == 0 || len == 0)
	{
		return ref new Platform::String(L"", 0);
	}

	size_t newLen = ::MultiByteToWideChar(CP_UTF8, 0, str, (int)len, nullptr, 0);
	if (newLen < 512)
	{
		std::array<wchar_t, 512> buf;
		::MultiByteToWideChar(CP_UTF8, 0, str, (int)len, buf.data(), (int)newLen + 1);
		return ref new Platform::String(buf.data(), (unsigned int)newLen);
	}
	else
	{
		std::vector<wchar_t> buf;
		buf.resize(newLen + 1);
		::MultiByteToWideChar(CP_UTF8, 0, str, (int)len, buf.data(), (int)newLen + 1);
		return ref new Platform::String(buf.data(), (unsigned int)newLen);
	}
}
