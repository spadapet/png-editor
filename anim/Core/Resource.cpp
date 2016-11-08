#include "pch.h"
#include "Core/Resource.h"
#include "Core/String.h"

Platform::String ^anim::Resource::GetString(Platform::String ^id)
{
	Windows::ApplicationModel::Resources::ResourceLoader ^loader =
		Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView();

	return loader->GetString(id);
}

std::string anim::Resource::GetStdString(Platform::String ^id)
{
	return anim::ConvertString(anim::Resource::GetString(id));
}

static Windows::Foundation::Uri ^GetUri(Platform::String ^id)
{
	if (wcscmp(id->Data(), L"ms-appx:") != 0)
	{
		id = Platform::String::Concat("ms-appx:///", id);
	}

	return ref new Windows::Foundation::Uri(id);
}

Platform::String ^anim::Resource::GetFile(Platform::String ^id)
{
	Windows::ApplicationModel::Resources::ResourceLoader ^loader =
		Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView();

	return loader->GetStringForUri(::GetUri(id));
}

std::string anim::Resource::GetStdFile(Platform::String ^id)
{
	return anim::ConvertString(anim::Resource::GetFile(id));
}

concurrency::task<Windows::Storage::StorageFile ^> anim::Resource::GetStorageFile(Platform::String ^id)
{
	Windows::ApplicationModel::Resources::ResourceLoader ^loader =
		Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView();

	return concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(::GetUri(id)));
}

concurrency::task<Platform::Array<unsigned char>^> anim::Resource::GetFileBytes(Platform::String ^id)
{
	auto fileTask = anim::Resource::GetStorageFile(id);

	auto openTask = fileTask.then([](Windows::Storage::StorageFile ^file)
	{
		if (file == nullptr)
		{
			throw ref new Platform::Exception(HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));
		}

		return file->OpenReadAsync();
	});

	auto readTask = openTask.then([](Windows::Storage::Streams::IRandomAccessStreamWithContentType ^stream)
	{
		Windows::Storage::Streams::IBuffer ^buffer = ref new Windows::Storage::Streams::Buffer((unsigned int)stream->Size);
		Windows::Storage::Streams::IInputStream ^input = stream->GetInputStreamAt(0);

		return input->ReadAsync(buffer, (unsigned int)stream->Size, Windows::Storage::Streams::InputStreamOptions::None);
	});

	auto arrayTask = readTask.then([](Windows::Storage::Streams::IBuffer ^buffer)
	{
		Windows::Storage::Streams::DataReader ^reader = Windows::Storage::Streams::DataReader::FromBuffer(buffer);
		Platform::Array<unsigned char> ^bytes = ref new Platform::Array<unsigned char>(buffer->Length);

		reader->ReadBytes(bytes);
		return bytes;
	});

	auto doneTask = arrayTask.then([](concurrency::task<Platform::Array<unsigned char> ^> prevTask) -> Platform::Array<unsigned char> ^
	{
		try
		{
			return prevTask.get();
		}
		catch (Platform::Exception ^ex)
		{
			return nullptr;
		}
	});

	return doneTask;
}
