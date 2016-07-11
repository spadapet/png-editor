#pragma once

#include "ViewModel/ViewProjectFolder.h"

namespace anim
{
	class AppState;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ViewAppState sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ViewAppState(AppState *parent);

	public:
		virtual ~ViewAppState();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;
		property Windows::Foundation::Collections::IVector<ViewProjectFolder ^> ^ProjectFolders { Windows::Foundation::Collections::IVector<ViewProjectFolder ^> ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void ModelPropertyChanged(const char *name);

		AppState *parent;
		EventCookie parentDisposedCookie;
		EventCookie parentChangedCookie;
		EventCookie projectFolderAddedCookie;
		EventCookie projectFolderRemovedCookie;

		Platform::Collections::Vector<ViewProjectFolder ^> ^projectFolders;
	};
}
