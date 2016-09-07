#include "pch.h"
#include "View/Utility/FlatProjectItem.h"
#include "View/Utility/FlatProjectList.h"
#include "ViewModel/IProjectItemVM.h"
#include "ViewModel/ProjectFolderVM.h"

anim::FlatProjectItem::FlatProjectItem()
	: FlatProjectItem(nullptr)
{
}

anim::FlatProjectItem::FlatProjectItem(FlatProjectList ^list)
	: weakList(list)
{
}

anim::FlatProjectItem::~FlatProjectItem()
{
}

void anim::FlatProjectItem::OnKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args)
{
	if (!args->Handled && this->Item != nullptr && this->List != nullptr)
	{
		ProjectFolderVM ^folder = this->Item->AsFolder;

		switch (args->Key)
		{
		case Windows::System::VirtualKey::Delete:
			args->Handled = true;
			if (this->Item->DeleteCommand != nullptr && this->Item->DeleteCommand->CanExecute(this->Item))
			{
				this->Item->DeleteCommand->Execute(this->Item);
			}
			break;

		case Windows::System::VirtualKey::Enter:
			args->Handled = true;
			if (this->Item->ActivateCommand != nullptr && this->Item->ActivateCommand->CanExecute(this->Item))
			{
				this->Item->ActivateCommand->Execute(this->Item);
			}
			break;

		case Windows::System::VirtualKey::Left:
			args->Handled = true;
			if (folder != nullptr && folder->ShowExpanded && folder->HasItems)
			{
				folder->ShowExpanded = false;
			}
			else if (this->Item->Parent != nullptr)
			{
				ProjectFolderVM ^parent = this->Item->Parent->AsFolder;
				if (parent != nullptr)
				{
					this->List->SelectSingle(parent);
				}
			}
			break;

		case Windows::System::VirtualKey::Right:
			args->Handled = true;
			if (folder != nullptr && !folder->ShowExpanded && folder->HasItems)
			{
				folder->ShowExpanded = true;
			}
			else if (folder != nullptr && folder->ShowExpanded && folder->HasItems && folder->Items->Size > 0)
			{
				IProjectItemVM ^child = folder->Items->GetAt(0);
				if (child != nullptr)
				{
					this->List->SelectSingle(child);
				}
			}
			break;
		}
	}

	Windows::UI::Xaml::Controls::ListBoxItem::OnKeyDown(args);
}

void anim::FlatProjectItem::OnPointerPressed(Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	Windows::UI::Xaml::Controls::ListBoxItem::OnPointerPressed(args);
}

void anim::FlatProjectItem::OnTapped(Windows::UI::Xaml::Input::TappedRoutedEventArgs ^args)
{
	Windows::UI::Xaml::Controls::ListBoxItem::OnTapped(args);
}

void anim::FlatProjectItem::OnDoubleTapped(Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs ^args)
{
	if (!args->Handled && this->Item != nullptr)
	{
		args->Handled = true;

		if (this->Item->ActivateCommand != nullptr && this->Item->ActivateCommand->CanExecute(nullptr))
		{
			this->Item->ActivateCommand->Execute(this->Item);
		}
	}

	Windows::UI::Xaml::Controls::ListBoxItem::OnDoubleTapped(args);
}

anim::FlatProjectList ^anim::FlatProjectItem::List::get()
{
	return this->weakList.Resolve<FlatProjectList>();
}

anim::IProjectItemVM ^anim::FlatProjectItem::Item::get()
{
	return dynamic_cast<IProjectItemVM ^>(this->DataContext);
}
