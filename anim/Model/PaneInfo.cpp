#include "pch.h"
#include "Model/PaneInfo.h"

anim::PaneInfo::PaneInfo(PaneType type, PaneUiFactory &&paneFactory)
	: type(type)
	, paneFactory(paneFactory)
{
}

anim::PaneInfo::~PaneInfo()
{
}

anim::PaneType anim::PaneInfo::GetType() const
{
	return this->type;
}

Windows::UI::Xaml::UIElement ^anim::PaneInfo::CreatePane() const
{
	return this->paneFactory != nullptr
		? this->paneFactory(this->type)
		: nullptr;
}
