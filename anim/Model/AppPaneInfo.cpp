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
