#include "pch.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"

anim::ProjectItem::ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<ProjectItem> parent)
	: item(item)
	, parent(parent)
	, app(parent->app)
{
}

anim::ProjectItem::ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<AppState> app)
	: item(item)
	, app(app)
{
}

anim::ProjectItem::~ProjectItem()
{
	this->GetAppState()->UnregisterProjectItem(this->item);
}

bool anim::ProjectItem::Equals(Windows::Storage::IStorageItem ^item) const
{
	if (item == nullptr)
	{
		return false;
	}

	return this->item == item || _wcsicmp(this->item->Path->Data(), item->Path->Data()) == 0;
}

Windows::Storage::IStorageItem ^anim::ProjectItem::GetItem() const
{
	return this->item;
}

void anim::ProjectItem::SetItem(Windows::Storage::IStorageItem ^item)
{
	if (item != this->item)
	{
		this->item = item;
		this->PropertyChanged.Notify("Item");
	}
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
	assert(this->GetParent() == nullptr);
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
