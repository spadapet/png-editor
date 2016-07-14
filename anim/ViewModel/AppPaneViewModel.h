#pragma once

#include "Core/Event.h"
#include "ViewModel/ProjectFolderViewModel.h"

namespace anim
{
	class AppPane;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AppPaneViewModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		AppPaneViewModel(AppPane *parent);

	public:
		AppPaneViewModel();
		virtual ~AppPaneViewModel();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Platform::String ^Name { Platform::String ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);

		AppPane *parent;
		EventCookie parentDisposedCookie;
	};
}
