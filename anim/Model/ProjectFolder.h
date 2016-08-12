#pragma once

#include "Model/ProjectItem.h"

namespace anim
{
	class ProjectFolder : public ProjectItem
	{
	public:
		ProjectFolder(Windows::Storage::StorageFolder ^folder, std::shared_ptr<ProjectFolder> parent);
		virtual ~ProjectFolder() override;

		Windows::Storage::StorageFolder ^GetFolder() const;
		void SetFolder(Windows::Storage::StorageFolder ^folder);
		const std::vector<std::shared_ptr<ProjectItem>> &GetItems();
		bool HasItems() const;
		bool CheckedForItems() const;

	private:
		void InitializeQuery();
		void Refresh();
		void Merge(std::vector<Windows::Storage::IStorageItem ^> newItems);

		Windows::Storage::StorageFolder ^folder;
		std::vector<std::shared_ptr<ProjectItem>> items;
		Windows::Storage::Search::StorageItemQueryResult ^query;
		Windows::Foundation::EventRegistrationToken queryChangedToken;
		bool initQuery;
	};
}
