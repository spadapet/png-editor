#include "pch.h"
#include "ViewModel/ViewModelBase.h"

png_editor::ViewModelBase::ViewModelBase()
{
}

png_editor::ViewModelBase::~ViewModelBase()
{
}

void png_editor::ViewModelBase::NotifyPropertyChanged(Platform::String ^name)
{
	this->PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(name));
}
