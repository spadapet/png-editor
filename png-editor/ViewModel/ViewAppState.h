#pragma once

#include "ViewModel/ViewModelBase.h"

class AppState;

namespace png_editor
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ViewAppState sealed : ViewModelBase
	{
	internal:
		ViewAppState(AppState *parent);

	public:
		virtual ~ViewAppState();

	private:
		AppState *parent;
	};
}
