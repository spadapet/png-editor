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
		FlatProjectItems(Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^items);
		virtual ~FlatProjectItems();

		property Windows::UI::Xaml::Interop::IBindableObservableVector ^BindableItems { Windows::UI::Xaml::Interop::IBindableObservableVector ^get(); }

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
		class Entry
		{
		public:
			Entry(FlatProjectItems ^owner, ProjectFolderVM ^folder);
			~Entry();

		private:
			ProjectFolderVM ^folder;
			Windows::Foundation::EventRegistrationToken folderChangedCookie;
			Windows::Foundation::EventRegistrationToken itemsChangedCookie;
		};

		void OnFolderChanged(Platform::String ^name);
		void OnFolderItemsChanged(ProjectFolderVM ^folder, Windows::UI::Xaml::Interop::IBindableObservableVector ^items, Windows::Foundation::Collections::IVectorChangedEventArgs ^args);

		Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^items;
		std::vector<Entry> entries;
		unsigned int cachedSize;
	};
}
