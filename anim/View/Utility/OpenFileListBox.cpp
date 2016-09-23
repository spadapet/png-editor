#include "pch.h"
#include "View/Utility/OpenFileListBox.h"
#include "View/Utility/OpenFileListItem.h"

anim::OpenFileListBox::OpenFileListBox()
{
}

void anim::OpenFileListBox::SetPopup(Windows::UI::Xaml::Controls::Primitives::Popup ^popup)
{
	if (popup != nullptr)
	{
		this->popup = popup;

		for (Platform::Object ^item : this->Items)
		{
			Platform::Object ^container = this->ContainerFromItem(item);
			OpenFileListItem ^listItem = dynamic_cast<OpenFileListItem ^>(container);

			if (listItem != nullptr)
			{
				listItem->SetPopup(popup);
			}
		}
	}
	else
	{
		this->popup = Platform::WeakReference();
	}
}

Windows::UI::Xaml::DependencyObject ^anim::OpenFileListBox::GetContainerForItemOverride()
{
	Windows::UI::Xaml::Controls::Primitives::Popup ^popup =
		this->popup.Resolve<Windows::UI::Xaml::Controls::Primitives::Popup>();
	assert(popup != nullptr);

	return ref new OpenFileListItem(popup);
}
