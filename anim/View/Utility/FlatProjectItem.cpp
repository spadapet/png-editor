#include "pch.h"
#include "View/Utility/FlatProjectItem.h"
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
	if (!args->Handled)
	{
		switch (args->Key)
		{
		case Windows::System::VirtualKey::Enter:
			if (this->Item->OnActivate())
			{
				args->Handled = true;
			}
			break;

		case Windows::System::VirtualKey::Left:
			if (this->Item->OnCollapse())
			{
				args->Handled = true;
			}
			// TODO: Else select parent item
			break;

		case Windows::System::VirtualKey::Right:
			if (this->Item->OnExpand())
			{
				args->Handled = true;
			}
			// TODO: Else select child item
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
	if (!args->Handled && this->Item->OnActivate())
	{
		args->Handled = true;
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
