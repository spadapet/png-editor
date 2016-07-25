#include "pch.h"
#include "Model/AppPaneInfo.h"

anim::AppPaneInfo::AppPaneInfo(AppPaneType type, AppPaneUiFactory &&paneFactory)
	: type(type)
	, paneFactory(paneFactory)
{
}

anim::AppPaneInfo::~AppPaneInfo()
{
	Disposed.Notify();
}

anim::AppPaneType anim::AppPaneInfo::GetType() const
{
	return this->type;
}

Windows::UI::Xaml::UIElement ^anim::AppPaneInfo::CreatePane() const
{
	return this->paneFactory != nullptr
		? this->paneFactory(this->type)
		: nullptr;
}
