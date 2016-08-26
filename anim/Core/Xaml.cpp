#include "pch.h"
#include "Core/Xaml.h"

void anim::AssertXamlDesigner()
{
	assert(anim::IsXamlDesigner());
}

bool anim::IsXamlDesigner()
{
	return Windows::ApplicationModel::DesignMode::DesignModeEnabled;
}

void anim::DisconnectDataTemplateBindings(Platform::Object ^obj)
{
	Windows::UI::Xaml::FrameworkElement ^elem = dynamic_cast<Windows::UI::Xaml::FrameworkElement ^>(obj);
	if (elem != nullptr)
	{
		Windows::UI::Xaml::IDataTemplateExtension ^data = Windows::UI::Xaml::DataTemplate::GetExtensionInstance(elem);
		if (data != nullptr)
		{
			data->ResetTemplate();
		}
	}
}
