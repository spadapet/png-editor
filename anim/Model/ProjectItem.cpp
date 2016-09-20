#include "pch.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"

anim::ProjectItem::ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<ProjectItem> parent)
	: item(safe_cast<Windows::Storage::IStorageItem2 ^>(item))
	, parent(parent)
	, app(parent->app)
	, cachedPath(item->Path)
{
}

anim::ProjectItem::ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<AppState> app)
	: item(safe_cast<Windows::Storage::IStorageItem2 ^>(item))
	, app(app)
	, cachedPath(item->Path)
{
}

anim::ProjectItem::~ProjectItem()
{
	this->GetAppState()->UnregisterProjectItem(this->cachedPath);
}

bool anim::ProjectItem::Equals(Windows::Storage::IStorageItem ^item) const
{
	return item != nullptr && this->item->IsEqual(item);
}

Platform::String ^anim::ProjectItem::GetName() const
{
	return this->item->Name;
}

Platform::String ^anim::ProjectItem::GetPath() const
{
	return this->item->Path;
}

Platform::String ^anim::ProjectItem::GetCachedPath() const
{
	return this->cachedPath;
}

Windows::Storage::IStorageItem ^anim::ProjectItem::GetItem() const
{
	return this->item;
}

void anim::ProjectItem::SetItem(Windows::Storage::IStorageItem ^item)
{
	if (item != this->item)
	{
		this->item = safe_cast<Windows::Storage::IStorageItem2 ^>(item);
		this->PropertyChanged.Notify(nullptr);
	}
}

bool anim::ProjectItem::UpdateItem()
{
	if (this->item->Path != this->cachedPath)
	{
		this->GetAppState()->UnregisterProjectItem(this->cachedPath);
		this->cachedPath = this->item->Path;
		this->GetAppState()->RegisterProjectItem(nullptr, this->item);

		this->PropertyChanged.Notify(nullptr);
		return true;
	}

	return false;
}

std::shared_ptr<anim::AppState> anim::ProjectItem::GetAppState() const
{
	return this->app.lock();
}

std::shared_ptr<anim::ProjectItem> anim::ProjectItem::GetParent() const
{
	return this->parent.lock();
}

void anim::ProjectItem::SetParent(std::shared_ptr<ProjectItem> parent)
{
	assert(this->parent.expired() || parent == nullptr);
	this->parent = parent;
}

bool anim::ProjectItem::IsFile() const
{
	return false;
}

bool anim::ProjectItem::IsFolder() const
{
	return false;
}
