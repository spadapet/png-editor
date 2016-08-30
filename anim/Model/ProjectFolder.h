#pragma once

#include "Model/ProjectItem.h"

namespace anim
{
	class ProjectFolder : public ProjectItem
	{
	public:
		ProjectFolder(Windows::Storage::StorageFolder ^folder, std::shared_ptr<ProjectFolder> parent);
		virtual ~ProjectFolder() override;

		// Overrides
		virtual void SetItem(Windows::Storage::IStorageItem ^item) override;
		virtual bool IsFolder() const override;

		Windows::Storage::StorageFolder ^GetFolder() const;
		void SetFolder(Windows::Storage::StorageFolder ^folder);
		const std::vector<std::shared_ptr<ProjectItem>> &GetItems();
		bool HasItems();

	private:
		void InitializeQuery();
		void InitializeItemQuery();
		void InitializeCountQuery();
		void RefreshItems();
		void RefreshCount();
		void Merge(std::vector<Windows::Storage::IStorageItem ^> newItems);
		std::shared_ptr<ProjectItem> MakeItem(Windows::Storage::IStorageItem ^item);

		Windows::Storage::StorageFolder ^folder;
		std::vector<std::shared_ptr<ProjectItem>> items;
		Windows::Storage::Search::StorageItemQueryResult ^query;
		Windows::Foundation::EventRegistrationToken queryChangedToken;
		bool mightHaveItems;
		bool initQuery;
		bool ranItemQuery;
		bool ranCountQuery;
	};
}
