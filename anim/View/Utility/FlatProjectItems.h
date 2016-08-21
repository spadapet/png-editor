#pragma once

namespace anim
{
	ref class ProjectFolderVM;
	interface class IProjectItemVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FlatProjectItems sealed
	{
	public:
		FlatProjectItems();
		FlatProjectItems(Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^roots);
		virtual ~FlatProjectItems();

		property Windows::UI::Xaml::Interop::IBindableObservableVector ^Items { Windows::UI::Xaml::Interop::IBindableObservableVector ^get(); }

	private:
		Platform::Collections::Vector<IProjectItemVM ^> ^flatItems;
	};
}
