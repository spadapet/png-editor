#include "pch.h"
#include "View/Utility/OpenFileComboBox.h"

anim::OpenFileComboBox::OpenFileComboBox()
	: loaded(false)
{
	this->Loaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenFileComboBox::OnLoaded);
}

void anim::OpenFileComboBox::OnLoaded(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^args)
{
	if (!this->loaded)
	{
		this->loaded = true;

		Windows::UI::Xaml::Controls::Primitives::Popup ^popup =
			safe_cast<Windows::UI::Xaml::Controls::Primitives::Popup ^>(
				this->GetTemplateChild("Popup"));

		popup->VerticalOffset = 32;
		popup->HorizontalOffset = 0;

		popup->RegisterPropertyChangedCallback(
			Windows::UI::Xaml::Controls::Primitives::Popup::VerticalOffsetProperty,
			ref new Windows::UI::Xaml::DependencyPropertyChangedCallback(this, &OpenFileComboBox::OnPopupVerticalOffsetChanged));

		popup->RegisterPropertyChangedCallback(
			Windows::UI::Xaml::Controls::Primitives::Popup::HorizontalOffsetProperty,
			ref new Windows::UI::Xaml::DependencyPropertyChangedCallback(this, &OpenFileComboBox::OnPopupVerticalOffsetChanged));
	}
}

void anim::OpenFileComboBox::OnPopupVerticalOffsetChanged(Windows::UI::Xaml::DependencyObject ^obj, Windows::UI::Xaml::DependencyProperty ^prop)
{
	Windows::UI::Xaml::Controls::Primitives::Popup ^popup = safe_cast<Windows::UI::Xaml::Controls::Primitives::Popup ^>(obj);
	if (popup->VerticalOffset != 32)
	{
		popup->VerticalOffset = 32;
	}
}

void anim::OpenFileComboBox::OnPopupHorizontalOffsetChanged(Windows::UI::Xaml::DependencyObject ^obj, Windows::UI::Xaml::DependencyProperty ^prop)
{
	Windows::UI::Xaml::Controls::Primitives::Popup ^popup = safe_cast<Windows::UI::Xaml::Controls::Primitives::Popup ^>(obj);
	if (popup->HorizontalOffset != 0)
	{
		popup->HorizontalOffset = 0;
	}
}
