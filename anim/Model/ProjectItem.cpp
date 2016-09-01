#include "pch.h"
#include "Model/AppState.h"
#include "Model/ProjectFolder.h"

anim::ProjectItem::ProjectItem(Windows::Storage::IStorageItem ^item, std::shared_ptr<ProjectItem> parent)
	: item(item)
	, parent(parent)
	, level(parent != nullptr ? parent->GetLevel() + 1 : 0)
{
}

anim::ProjectItem::~ProjectItem()
{
	this->GetAppState()->UnregisterProjectItem(this->shared_from_this());
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
	std::shared_ptr<ProjectItem> parent = this->GetParent();
	if (parent != nullptr)
	{
		return parent->GetAppState();
	}

	return nullptr;
}

std::shared_ptr<anim::ProjectItem> anim::ProjectItem::GetParent() const
{
	return this->parent.lock();
}

int anim::ProjectItem::GetLevel() const
{
	return this->level;
}

bool anim::ProjectItem::IsFile() const
{
	return false;
}

bool anim::ProjectItem::IsFolder() const
{
	return false;
}
