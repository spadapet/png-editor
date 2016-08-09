#include "pch.h"
#include "Core/Designer.h"

void anim::AssertXamlDesigner()
{
	assert(anim::IsXamlDesigner());
}

bool anim::IsXamlDesigner()
{
	return Windows::ApplicationModel::DesignMode::DesignModeEnabled;
}
