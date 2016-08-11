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
		std::shared_ptr<ProjectFolder> GetParent() const;
		int GetLevel() const;

	protected:
		void SetItem(Windows::Storage::IStorageItem ^item);

	private:
		Windows::Storage::IStorageItem ^item;
		std::weak_ptr<ProjectFolder> parent;
		int level;
	};
}
