#include "pch.h"
#include "Model/RootProjectFolder.h"

anim::RootProjectFolder::RootProjectFolder(Windows::Storage::StorageFolder ^folder, std::shared_ptr<AppState> app)
	: ProjectFolder(folder, nullptr)
	, app(app)
{
}

anim::RootProjectFolder::~RootProjectFolder()
{
}

std::shared_ptr<anim::AppState> anim::RootProjectFolder::GetAppState() const
{
	return this->app.lock();
}
