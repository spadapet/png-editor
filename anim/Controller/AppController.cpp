#include "pch.h"
#include "Controller/AppController.h"
#include "Model/AppState.h"
#include "Model/ProjectFile.h"

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

void anim::AppController::OpenFile()
{
	Windows::Storage::Pickers::FileOpenPicker ^dialog = ref new Windows::Storage::Pickers::FileOpenPicker();
	dialog->FileTypeFilter->Append(".png");

	std::shared_ptr<AppState> app = this->app;
	auto pickTask = concurrency::create_task(dialog->PickSingleFileAsync());

	pickTask.then([app](Windows::Storage::StorageFile ^file)
	{
		std::shared_ptr<ProjectFile> model = std::dynamic_pointer_cast<ProjectFile>(app->RegisterProjectItem(nullptr, file));
		if (model != nullptr)
		{
			app->EditFile(model);
		}
	}, concurrency::task_continuation_context::use_current());
}
