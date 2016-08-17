#include "pch.h"
#include "View/Utility/ITreeHost.h"
#include "View/Utility/TreeRow.h"

anim::TreeRow::TreeRow()
	: pointerOverToken(0)
{
	Platform::WeakReference weakOwner(this);

	this->pointerOverToken = this->RegisterPropertyChangedCallback(this->IsPointerOverProperty,
		ref new Windows::UI::Xaml::DependencyPropertyChangedCallback(
			[weakOwner](Windows::UI::Xaml::DependencyObject ^sender, Windows::UI::Xaml::DependencyProperty ^prop)
	{
		TreeRow ^owner = weakOwner.Resolve<TreeRow>();
		if (owner != nullptr)
		{
			owner->UpdateVisualState();
		}
	}));
}

anim::TreeRow::~TreeRow()
{
	this->UnregisterPropertyChangedCallback(this->IsPointerOverProperty, this->pointerOverToken);
}

bool anim::TreeRow::IsSelected::get()
{
	return this->GetTreeHost()->SelectedTreeRow == this;
}

void anim::TreeRow::IsSelected::set(bool value)
{
	this->UpdateVisualState();
}

void anim::TreeRow::UpdateVisualState()
{
	Platform::String ^name = "Normal";

	if (this->IsPointerOver)
	{
		name = this->IsSelected ? "SelectedPointerOver" : "PointerOver";
	}
	else if (this->IsSelected)
	{
		name = "Selected";
	}

	Windows::UI::Xaml::VisualStateManager::GoToState(this, name, true);
}

anim::ITreeHost ^anim::TreeRow::GetTreeHost()
{
	return dynamic_cast<ITreeHost ^>(this->DataContext);
}
