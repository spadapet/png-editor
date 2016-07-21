#include "pch.h"
#include "Core/Resource.h"

Platform::String ^anim::Resource::GetString(Platform::String ^id)
{
	Windows::ApplicationModel::Resources::ResourceLoader ^loader =
		Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView();

	return loader->GetString(id);

}
