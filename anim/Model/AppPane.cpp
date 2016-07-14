#include "pch.h"
#include "Model/AppPane.h"

anim::AppPane::AppPane(AppPaneType type, AppPaneUiFactory &&paneFactory)
	: type(type)
	, paneFactory(paneFactory)
{
}

anim::AppPane::~AppPane()
{
	Disposed.Notify();
}

anim::AppPaneType anim::AppPane::GetType() const
{
	return this->type;
}
