#pragma once

#include "Core/Event.h"

namespace anim
{
	class GraphDevice
	{
	public:
		GraphDevice();
		~GraphDevice();

		SimpleEvent DeviceReset;

		ComPtr<IDXGIFactory5> GetFactory() const;
		ComPtr<ID3D11Device4> GetDevice() const;

	private:
		ComPtr<IDXGIFactory5> factory;
		ComPtr<ID3D11Device4> device;
	};
}
