#pragma once

#include "Core/Event.h"

namespace anim
{
	class AppState;
	interface class IOpenFileVM;
	ref class OpenNullFileVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class OpenFileTabsVM sealed : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		OpenFileTabsVM(std::shared_ptr<AppState> app);
		std::shared_ptr<AppState> GetApp() const;

	public:
		OpenFileTabsVM();
		virtual ~OpenFileTabsVM();

		// INotifyPropertyChanged
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

		property Windows::Foundation::Collections::IVector<IOpenFileVM ^> ^Files { Windows::Foundation::Collections::IVector<IOpenFileVM ^> ^get(); }
		property bool HasFiles { bool get(); }
		property IOpenFileVM ^FocusFile { IOpenFileVM ^get(); void set(IOpenFileVM ^value); }
		property IOpenFileVM ^FocusFileOrNull { IOpenFileVM ^get(); void set(IOpenFileVM ^value); }
		property Windows::UI::Xaml::Controls::ListBox ^TabsList { Windows::UI::Xaml::Controls::ListBox ^get(); void set(Windows::UI::Xaml::Controls::ListBox ^value); }

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
		Windows::Foundation::EventRegistrationToken focusFilePropertyChangedCookie;
		IOpenFileVM ^focusFile;
		OpenNullFileVM ^nullFile;
		Platform::WeakReference tabsList;
	};
}
