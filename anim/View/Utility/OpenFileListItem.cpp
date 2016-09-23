#include "pch.h"
#include "Core/Thread.h"
#include "View/Utility/OpenFileListItem.h"

anim::OpenFileListItem::OpenFileListItem(Windows::UI::Xaml::Controls::Primitives::Popup ^popup)
{
	if (popup != nullptr)
	{
		this->popup = Platform::WeakReference(popup);
	}
}

anim::OpenFileListItem::OpenFileListItem()
{
}

void anim::OpenFileListItem::SetPopup(Windows::UI::Xaml::Controls::Primitives::Popup ^popup)
{
	if (popup != nullptr)
	{
		this->popup = popup;
	}
	else
	{
		this->popup = Platform::WeakReference();
	}
}

void anim::OpenFileListItem::OnKeyDown(Windows::UI::Xaml::Input::KeyRoutedEventArgs ^args)
{
	if (args->Key == Windows::System::VirtualKey::Enter)
	{
		ClosePopup();
	}

	Windows::UI::Xaml::Controls::ListBoxItem::OnKeyDown(args);
}

void anim::OpenFileListItem::OnPointerReleased(Windows::UI::Xaml::Input::PointerRoutedEventArgs ^args)
{
	Windows::UI::Xaml::Controls::ListBoxItem::OnPointerReleased(args);

	ClosePopup();
}

void anim::OpenFileListItem::ClosePopup()
{
	Platform::WeakReference weakPopup = this->popup;

	anim::PostToMainThread([weakPopup]()
	{
		Windows::UI::Xaml::Controls::Primitives::Popup ^popup =
			weakPopup.Resolve<Windows::UI::Xaml::Controls::Primitives::Popup>();

		if (popup != nullptr)
		{
			popup->IsOpen = false;
		}
	});
}
