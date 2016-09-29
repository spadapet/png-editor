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

		// File props
		std::shared_ptr<ProjectFile> GetFile() const;
		virtual bool IsDirty() const = 0;
		virtual bool IsLoaded() const = 0;
		virtual bool IsFatalError() const = 0;
		virtual std::string GetFatalError() const = 0;

		// Undo
		virtual size_t GetUndoCount() const = 0;
		virtual size_t GetRedoCount() const = 0;
		virtual void Undo() = 0;
		virtual void Redo() = 0;

	private:
		std::shared_ptr<ProjectFile> file;
	};
}
