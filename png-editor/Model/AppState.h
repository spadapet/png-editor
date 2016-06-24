#pragma once

namespace png_editor
{
	ref class ViewAppState;
}

class AppState
{
public:
	AppState();
	~AppState();

	png_editor::ViewAppState ^GetViewModel() const;

private:
	png_editor::ViewAppState ^viewState;
};
