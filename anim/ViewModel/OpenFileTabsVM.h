#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;
	interface class IOpenFileVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenFileTabsVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		OpenFileTabsVM(std::shared_ptr<AppState> app);

	public:
		OpenFileTabsVM();
		virtual ~OpenFileTabsVM();

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		property Windows::Foundation::Collections::IVector<IOpenFileVM ^> ^Files { Windows::Foundation::Collections::IVector<IOpenFileVM ^> ^get(); }
		property IOpenFileVM ^FocusFile { IOpenFileVM ^get(); void set(IOpenFileVM ^value); }
		property Windows::UI::Xaml::UIElement ^FocusFileUserInterface { Windows::UI::Xaml::UIElement ^get(); }

	private:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
		void OnFileOpened(std::shared_ptr<OpenFile> file);
		void OnFileClosed(std::shared_ptr<OpenFile> file);
		bool OnFileFocus(std::shared_ptr<OpenFile> file);
		void ResetFiles();

		std::shared_ptr<AppState> app;
		EventCookie fileOpenedCookie;
		EventCookie fileClosedCookie;
		EventCookie fileFocusCookie;
		Platform::Collections::Vector<IOpenFileVM ^> ^files;
		IOpenFileVM ^focusFile;
	};
}
