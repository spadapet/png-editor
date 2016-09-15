#pragma once

namespace anim
{
	void AssertXamlDesigner();
	bool IsXamlDesigner();
	void DisconnectDataTemplateBindings(Platform::Object ^obj);

	template<class T>
	T ^GetDataFromSender(Platform::Object ^sender);
}

template<class T>
T ^anim::GetDataFromSender(Platform::Object ^sender)
{
	Windows::UI::Xaml::DependencyObject ^elem = dynamic_cast<Windows::UI::Xaml::DependencyObject ^>(sender);
	assert(elem != nullptr);

	if (elem != nullptr)
	{
		Platform::Object ^dc = elem->GetValue(Windows::UI::Xaml::Controls::Control::DataContextProperty);
		return dynamic_cast<T ^>(dc);
	}

	return nullptr;
}
