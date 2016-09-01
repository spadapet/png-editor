#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;

	class ProjectItem : public std::enable_shared_from_this<ProjectItem>
	{
	public:
		ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<ProjectItem> parent);
		virtual ~ProjectItem();

		ChangedEvent PropertyChanged;

		bool Equals(Windows::Storage::IStorageItem ^item) const;
		Windows::Storage::IStorageItem ^GetItem() const;
		virtual void SetItem(Windows::Storage::IStorageItem ^item);
		virtual std::shared_ptr<AppState> GetAppState() const;
		std::shared_ptr<ProjectItem> GetParent() const;
		int GetLevel() const;
		virtual bool IsFile() const;
		virtual bool IsFolder() const;

	private:
		Windows::Storage::IStorageItem ^item;
		std::weak_ptr<ProjectItem> parent;
		int level;
	};
}
