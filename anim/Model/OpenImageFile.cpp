#include "pch.h"
#include "Model/OpenImageFile.h"
#include "Model/ProjectFile.h"

anim::OpenImageFile::OpenImageFile(std::shared_ptr<ProjectFile> file)
	: OpenFile(file)
	, initialized(false)
{
}

anim::OpenImageFile::~OpenImageFile()
{
}

void anim::OpenImageFile::Initialize()
{
	if (this->initialized)
	{
		return;
	}

	this->initialized = true;

	OpenFile::Initialize();

	Windows::Storage::StorageFile ^file = this->GetFile()->GetFile();

	auto openTask = concurrency::create_task(file->OpenReadAsync());

	auto readTask = openTask.then([](Windows::Storage::Streams::IRandomAccessStreamWithContentType ^stream)
	{
		Windows::Storage::Streams::IBuffer ^buffer = ref new Windows::Storage::Streams::Buffer((unsigned int)stream->Size);
		Windows::Storage::Streams::IInputStream ^input = stream->GetInputStreamAt(0);

		return input->ReadAsync(buffer, (unsigned int)stream->Size, Windows::Storage::Streams::InputStreamOptions::None);
	});

	auto parseTask = readTask.then([](Windows::Storage::Streams::IBuffer ^buffer)
	{
		Windows::Storage::Streams::DataReader ^reader = Windows::Storage::Streams::DataReader::FromBuffer(buffer);
		Platform::Array<unsigned char> ^bytes = ref new Platform::Array<unsigned char>(buffer->Length);

		reader->ReadBytes(bytes);

		png_image image;
		::memset(&image, 0, sizeof(image));
		image.version = PNG_IMAGE_VERSION;

		if (::png_image_begin_read_from_memory(&image, bytes->Data, bytes->Length))
		{
			image.format = PNG_FORMAT_RGBA;
			::png_image_free(&image);
		}
	});

	auto doneTask = parseTask.then([](concurrency::task<void> parseTask)
	{
		try
		{
			parseTask.get();
		}
		catch (Platform::Exception ^ex)
		{
			// doesn't matter if state can't be loaded
		}
	});
}

void anim::OpenImageFile::Destroy()
{
	OpenFile::Destroy();
}

bool anim::OpenImageFile::IsDirty() const
{
	return false;
}

bool anim::OpenImageFile::IsLoaded() const
{
	return false;
}
