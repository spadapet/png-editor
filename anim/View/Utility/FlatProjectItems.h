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
		FlatProjectItems(Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^rootItems);
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
		struct Entry
		{
			Entry();
			Entry(Entry &&rhs);
			~Entry();

			Entry &operator=(Entry &&rhs);

			ProjectFolderVM ^folder;
			bool addedChildren;
			Windows::Foundation::EventRegistrationToken folderChangedCookie;
			Windows::Foundation::EventRegistrationToken itemsChangedCookie;

		private:
			Entry(const Entry &rhs);

			void Destroy();
		};

		ref class Iterator sealed : Windows::UI::Xaml::Interop::IBindableIterator
		{
		public:
			Iterator(Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^items);

			// IBindableIterator
			virtual property Platform::Object ^Current { Platform::Object ^get(); }
			virtual property bool HasCurrent { bool get(); }
			virtual bool MoveNext();

		private:
			Windows::Foundation::Collections::IVector<IProjectItemVM ^> ^items;
			Windows::UI::Xaml::Interop::IBindableIterator ^nested;
			unsigned int pos;
		};

		void OnRootItemsChanged(Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^sender, Windows::Foundation::Collections::IVectorChangedEventArgs ^args);
		void OnRootFolderChanged(ProjectFolderVM ^folder, Platform::String ^name);
		void OnRootFolderItemsChanged(ProjectFolderVM ^folder, Windows::Foundation::Collections::IVectorChangedEventArgs ^args);

		void Validate();
		void ResetRootEntries();
		void AddRootEntry(IProjectItemVM ^item, unsigned int index = INVALID_UINT);
		void RemoveRootEntry(unsigned int index);

		void AddRootFolderChildren(ProjectFolderVM ^folder, bool force = false);
		void RemoveRootFolderChildren(ProjectFolderVM ^folder, bool force = false);
		void NotifyVectorChanged(Windows::Foundation::Collections::CollectionChange change, unsigned int index = 0, IProjectItemVM ^item = nullptr);

		unsigned int FlatIndexOfRootFolder(ProjectFolderVM ^folder, unsigned int *realIndex = nullptr);
		unsigned int FlatIndexOfRootIndex(unsigned int index);

		Windows::Foundation::Collections::IObservableVector<IProjectItemVM ^> ^rootItems;
		Windows::Foundation::EventRegistrationToken rootItemsChangedCookie;
		std::vector<Entry> entries;
		unsigned int size;
		int blockNotifications;

		// Makes GetAt() faster when we know what item is going to be requested
		unsigned int cachedIndex;
		IProjectItemVM ^cachedItem;
	};
}
