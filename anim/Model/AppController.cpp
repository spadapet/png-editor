#include "pch.h"
#include "Model/AppController.h"
#include "Model/AppState.h"

anim::AppController::AppController(std::shared_ptr<AppState> app)
	: app(app)
{
}

anim::AppController::~AppController()
{
}

void anim::AppController::AddProjectFolder()
{
	auto dialog = ref new Windows::Storage::Pickers::FolderPicker();
	dialog->FileTypeFilter->Append("*");

	auto pickTask = concurrency::create_task(dialog->PickSingleFolderAsync());
	auto app = this->app;

	pickTask.then([app](Windows::Storage::StorageFolder ^folder)
	{
		app->AddProjectFolder(folder);
	}, concurrency::task_continuation_context::use_current());
}

void anim::AppController::OpenFile()
{
}
