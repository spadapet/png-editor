#pragma once

namespace anim
{
	ref class ProjectFolderVM;
	interface class IProjectItemVM;

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class FlatProjectItems sealed : Windows::UI::Xaml::Interop::IBindableObservableVector
	{
	public:
		FlatProjectItems();
		FlatProjectItems(Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^roots);
		virtual ~FlatProjectItems();

		// IBindableObservableVector
		virtual event Windows::UI::Xaml::Interop::BindableVectorChangedEventHandler ^VectorChanged;
		virtual property unsigned int Size { unsigned int get(); }
		virtual Windows::UI::Xaml::Interop::IBindableIterator ^First();
		virtual Platform::Object ^GetAt(unsigned int index);
		virtual Windows::UI::Xaml::Interop::IBindableVectorView ^GetView();
		virtual bool IndexOf(Platform::Object ^value, unsigned int *index);
		virtual void SetAt(unsigned int index, Platform::Object ^value);
		virtual void InsertAt(unsigned int index, Platform::Object ^value);
		virtual void RemoveAt(unsigned int index);
		virtual void Append(Platform::Object ^value);
		virtual void RemoveAtEnd();
		virtual void Clear();

	private:
		Windows::Foundation::Collections::IVector<ProjectFolderVM ^> ^roots;
	};
}
