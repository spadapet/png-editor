#pragma once

namespace anim
{
	namespace Resource
	{
		// Strings
		Platform::String ^GetString(Platform::String ^id);
		std::string GetStdString(Platform::String ^id);

		// File paths
		Platform::String ^GetFile(Platform::String ^id);
		std::string GetStdFile(Platform::String ^id);

		// Async files
		concurrency::task<Windows::Storage::StorageFile ^> GetStorageFile(Platform::String ^id);
		concurrency::task<Platform::Array<unsigned char> ^> GetFileBytes(Platform::String ^id);
	}
}
