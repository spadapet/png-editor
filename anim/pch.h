#pragma once

#include <cassert>
#include <forward_list>
#include <sstream>

#include <collection.h>
#include <ppltasks.h>
#include <windows.foundation.collections.h>

namespace anim
{
	static const size_t INVALID_SIZE = (size_t)-1;
	static const unsigned int INVALID_UINT = (unsigned int)-1;

	enum class Tri
	{
		False,
		True,
		Unknown
	};
}
