#pragma once

#include "Model/ProjectItem.h"

namespace anim
{
	class OpenFile;

	class ProjectFile : public ProjectItem
	{
	public:
		ProjectFile(Windows::Storage::StorageFile ^file, std::shared_ptr<ProjectItem> parent);
		ProjectFile(Windows::Storage::StorageFile ^file, std::shared_ptr<AppState> app);
		virtual ~ProjectFile() override;

		// Overrides
		virtual void SetItem(Windows::Storage::IStorageItem ^item) override;
		virtual bool IsFile() const override;

		Windows::Storage::StorageFile ^GetFile() const;
		void SetFile(Windows::Storage::StorageFile ^file);

		bool IsOpen() const;
		std::shared_ptr<OpenFile> GetOpenFile() const;
		void SetOpenFile(std::shared_ptr<OpenFile> openFile);

	private:
		Windows::Storage::StorageFile ^file;
		std::weak_ptr<OpenFile> openFile;
	};
}
