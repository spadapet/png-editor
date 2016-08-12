#pragma once

#include "Core/Event.h"

namespace anim
{
	class ProjectFolder;

	class ProjectItem : public std::enable_shared_from_this<ProjectItem>
	{
	public:
		ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<ProjectFolder> parent);
		virtual ~ProjectItem();

		ChangedEvent PropertyChanged;

		Windows::Storage::IStorageItem ^GetItem() const;
		virtual void SetItem(Windows::Storage::IStorageItem ^item);
		std::shared_ptr<ProjectFolder> GetParent() const;
		int GetLevel() const;
		virtual bool IsFile() const;
		virtual bool IsFolder() const;

	private:
		Windows::Storage::IStorageItem ^item;
		std::weak_ptr<ProjectFolder> parent;
		int level;
	};
}
