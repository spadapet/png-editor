#pragma once

namespace anim
{
	std::string ConvertString(Platform::String ^str);
	Platform::String ^ConvertString(const std::string &str);
	Platform::String ^ConvertString(const char *str, size_t len = INVALID_SIZE);

	namespace Resource
	{
		Platform::String ^GetString(Platform::String ^id);
		std::string GetStdString(Platform::String ^id);
		Platform::String ^GetFile(Platform::String ^id);
		std::string GetStdFile(Platform::String ^id);
		concurrency::task<Windows::Storage::StorageFile ^> GetStorageFile(Platform::String ^id);
		concurrency::task<Platform::Array<unsigned char> ^> GetFileBytes(Platform::String ^id);
	}
}
