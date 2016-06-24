#pragma once

namespace png_editor
{
	[Windows::Foundation::Metadata::WebHostHidden]
	private ref class ViewModelBase : Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	internal:
		ViewModelBase();

	public:
		virtual ~ViewModelBase();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler ^PropertyChanged;

	protected:
		void NotifyPropertyChanged(Platform::String ^name = nullptr);
	};
}
