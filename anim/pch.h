#pragma once

#define NOMINMAX

// CRT
#include <algorithm>
#include <cassert>
#include <forward_list>
#include <sstream>

// Windows
#include <collection.h>
#include <ppltasks.h>
#include <windows.foundation.collections.h>
#include <windows.ui.xaml.media.dxinterop.h>
#include <wrl.h>

// DirectX
#include <dxgi1_5.h>
#include <d3d11_4.h>
#include <d2d1_3.h>

// Libraries
#include "../libpng/png.h"
#include "../zlib/zlib.h"

using Microsoft::WRL::ComPtr;

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
