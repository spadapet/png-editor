#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;

	class ProjectItem : public std::enable_shared_from_this<ProjectItem>
	{
	public:
		ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<ProjectItem> parent);
		ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<AppState> app);
		virtual ~ProjectItem();

		ChangedEvent PropertyChanged;

		bool Equals(Windows::Storage::IStorageItem ^item) const;

		Platform::String ^GetName() const;
		Platform::String ^GetPath() const;
		Platform::String ^GetCachedPath() const;
		Windows::Storage::IStorageItem ^GetItem() const;
		virtual void SetItem(Windows::Storage::IStorageItem ^item);
		virtual bool UpdateItem();
		std::shared_ptr<AppState> GetAppState() const;
		std::shared_ptr<ProjectItem> GetParent() const;
		void SetParent(std::shared_ptr<ProjectItem> parent);

		virtual bool IsFile() const;
		virtual bool IsFolder() const;

	private:
		Windows::Storage::IStorageItem2 ^item;
		std::weak_ptr<ProjectItem> parent;
		std::weak_ptr<AppState> app;
		Platform::String ^cachedPath;
	};
}
