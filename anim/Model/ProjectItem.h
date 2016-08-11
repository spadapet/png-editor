#pragma once

namespace anim
{
	class ProjectFolder;

	class ProjectItem : public std::enable_shared_from_this<ProjectItem>
	{
	public:
		ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<ProjectFolder> parent);
		virtual ~ProjectItem();

		Windows::Storage::IStorageItem ^GetItem() const;
		std::shared_ptr<ProjectFolder> GetParent() const;
		int GetLevel() const;

	private:
		Windows::Storage::IStorageItem ^item;
		std::weak_ptr<ProjectFolder> parent;
		int level;
	};
}
