#include "pch.h"
#include "View/Utility/ITreeHost.h"
#include "View/Utility/TreeRow.h"

anim::TreeRow::TreeRow()
	: pointerOver(false)
{
}

anim::TreeRow::~TreeRow()
{
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

	if (pointerOver)
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
