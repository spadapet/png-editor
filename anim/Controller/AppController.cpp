#include "pch.h"
#include "Controller/AppController.h"
#include "Model/AppState.h"

anim::AppController::AppController(std::shared_ptr<AppState> app)
	: app(app)
{
}

void anim::AppController::AddProjectFolder()
{
	Windows::Storage::Pickers::FolderPicker ^dialog = ref new Windows::Storage::Pickers::FolderPicker();
	dialog->FileTypeFilter->Append("*");

	std::shared_ptr<AppState> app = this->app;
	auto pickTask = concurrency::create_task(dialog->PickSingleFolderAsync());

	pickTask.then([app](Windows::Storage::StorageFolder ^folder)
	{
		app->AddProjectFolder(folder);
	}, concurrency::task_continuation_context::use_current());
}

void anim::AppController::RemoveProjectFolder(std::shared_ptr<ProjectFolder> folder)
{
}

void anim::AppController::OpenFile()
{
}
