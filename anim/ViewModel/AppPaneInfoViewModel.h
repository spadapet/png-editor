#pragma once

#include "Core/Event.h"
#include "ViewModel/ProjectFolderViewModel.h"

namespace anim
{
	class AppPaneInfo;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AppPaneInfoViewModel sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		AppPaneInfoViewModel(AppPaneInfo *parent);

	public:
		AppPaneInfoViewModel();
		virtual ~AppPaneInfoViewModel();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Platform::String ^Name { Platform::String ^get(); }
		property Windows::UI::Xaml::Media::ImageSource ^Icon { Windows::UI::Xaml::Media::ImageSource ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ModelPropertyChanged(const char *name);

		AppPaneInfo *parent;
		EventCookie parentDisposedCookie;
		EventCookie parentChangedCookie;
		Windows::UI::Xaml::Media::ImageSource ^icon;
	};
}
