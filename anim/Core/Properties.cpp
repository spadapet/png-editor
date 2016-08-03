#include "pch.h"
#include "Core/Properties.h"

static Windows::UI::Xaml::DependencyProperty ^visualStateProperty = Windows::UI::Xaml::DependencyProperty::RegisterAttached(
	"VisualState",
	Windows::UI::Xaml::Interop::TypeName(Platform::String::typeid),
	Windows::UI::Xaml::Interop::TypeName(anim::Properties::typeid),
	ref new Windows::UI::Xaml::PropertyMetadata(nullptr, ref new Windows::UI::Xaml::PropertyChangedCallback(&anim::Properties::OnVisualStateChanged)));

Windows::UI::Xaml::DependencyProperty ^anim::Properties::VisualStateProperty::get()
{
	return visualStateProperty;
}

Platform::String ^anim::Properties::GetVisualState(Windows::UI::Xaml::UIElement ^element)
{
	return (Platform::String ^)element->GetValue(anim::Properties::VisualStateProperty);
}

void anim::Properties::SetVisualState(Windows::UI::Xaml::UIElement ^element, Platform::String ^value)
{
	element->SetValue(anim::Properties::VisualStateProperty, value);
}

void anim::Properties::OnVisualStateChanged(Windows::UI::Xaml::DependencyObject ^sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs ^args)
{
	Platform::String ^state = dynamic_cast<Platform::String ^>(args->NewValue);
	Windows::UI::Xaml::Controls::Control ^control = dynamic_cast<Windows::UI::Xaml::Controls::Control ^>(sender);

	if (state != nullptr && control != nullptr)
	{
		Windows::UI::Xaml::VisualStateManager::GoToState(control, state, true);
	}
}
