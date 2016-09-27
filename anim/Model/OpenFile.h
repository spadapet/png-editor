#pragma once

#include "Core/Event.h"

namespace anim
{
	class ProjectFile;

	class OpenFile : public std::enable_shared_from_this<OpenFile>
	{
	public:
		OpenFile(std::shared_ptr<ProjectFile> file);
		~OpenFile();

		virtual void Initialize();
		virtual void Destroy();

		// Events
		ChangedEvent PropertyChanged;

		std::shared_ptr<ProjectFile> GetFile() const;
		virtual bool IsDirty() const = 0;
		virtual bool IsLoaded() const = 0;

	private:
		std::shared_ptr<ProjectFile> file;
	};
}
