#include "pch.h"
#include "Core/String.h"
#include "Core/Thread.h"
#include "Model/Image.h"
#include "Model/OpenImageFile.h"
#include "Model/ProjectFile.h"

anim::OpenImageFile::OpenImageFile(std::shared_ptr<ProjectFile> file)
	: OpenFile(file)
	, initialized(false)
	, loading(true)
	, dirty(false)
{
}

anim::OpenImageFile::~OpenImageFile()
{
}

void anim::OpenImageFile::Reset(std::shared_ptr<Image> image, std::string fatalError)
{
	assert(anim::IsMainThread());

	this->image = image;
	this->fatalError = fatalError;
	this->loading = false;

	this->PropertyChanged.Notify(nullptr);
}

void anim::OpenImageFile::Initialize()
{
	if (this->initialized)
	{
		return;
	}

	this->initialized = true;

	OpenFile::Initialize();

	std::shared_ptr<OpenImageFile> owner = std::dynamic_pointer_cast<OpenImageFile>(this->shared_from_this());
	Windows::Storage::StorageFile ^file = this->GetFile()->GetFile();

	auto openTask = concurrency::create_task(file->OpenReadAsync());

	auto readTask = openTask.then([](Windows::Storage::Streams::IRandomAccessStreamWithContentType ^stream)
	{
		Windows::Storage::Streams::IBuffer ^buffer = ref new Windows::Storage::Streams::Buffer((unsigned int)stream->Size);
		Windows::Storage::Streams::IInputStream ^input = stream->GetInputStreamAt(0);

		return input->ReadAsync(buffer, (unsigned int)stream->Size, Windows::Storage::Streams::InputStreamOptions::None);
	});

	auto imageTask = readTask.then([](Windows::Storage::Streams::IBuffer ^buffer) -> std::shared_ptr<Image>
	{
		Windows::Storage::Streams::DataReader ^reader = Windows::Storage::Streams::DataReader::FromBuffer(buffer);
		Platform::Array<unsigned char> ^bytes = ref new Platform::Array<unsigned char>(buffer->Length);

		reader->ReadBytes(bytes);
		return std::make_shared<Image>(bytes->Data, bytes->Length);
	});

	struct Result
	{
		std::string fatalError;
		std::shared_ptr<Image> image;
	};

	auto resultTask = imageTask.then([](concurrency::task<std::shared_ptr<Image>> prevTask) -> Result
	{
		Result result;

		try
		{
			result.image = prevTask.get();
			if (result.image == nullptr)
			{
				throw ref new Platform::Exception(E_FAIL, anim::Resource::GetString("ErrorNullImage"));
			}
		}
		catch (Platform::Exception ^ex)
		{
			result.fatalError = anim::ConvertString(ex->Message);
		}

		return result;
	});

	auto doneTask = resultTask.then([owner](Result result)
	{
		assert(owner->loading);
		owner->Reset(result.image, result.fatalError);
	}, concurrency::task_continuation_context::use_current());
}

void anim::OpenImageFile::Destroy()
{
	OpenFile::Destroy();
}

bool anim::OpenImageFile::IsDirty() const
{
	return this->dirty;
}

bool anim::OpenImageFile::IsLoaded() const
{
	return !this->loading;
}

bool anim::OpenImageFile::IsFatalError() const
{
	return !this->fatalError.empty();
}

std::string anim::OpenImageFile::GetFatalError() const
{
	return this->fatalError;
}

size_t anim::OpenImageFile::GetUndoCount() const
{
	return 0;
}

size_t anim::OpenImageFile::GetRedoCount() const
{
	return 0;
}

void anim::OpenImageFile::Undo()
{
}

void anim::OpenImageFile::Redo()
{
}
