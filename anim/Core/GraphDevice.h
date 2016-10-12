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

		void Initialize();
		bool IsValid() const;
		bool ResetIfNeeded();

		IDXGIFactory5 *GetFactory() const;
		IDXGIDevice4 *GetDevice() const;
		ID3D11Device4 *GetDevice3d() const;
		ID3D11DeviceContext3 *GetContext3d() const;
		ID2D1Factory3 *GetFactory2d() const;
		ID2D1Device2 *GetDevice2d() const;
		ID2D1DeviceContext2 *GetContext2d() const;

	private:
		bool InternalInitialize();
		void Reset(bool reinit);

		bool valid;
		ComPtr<IDXGIFactory5> factory;
		ComPtr<IDXGIDevice4> device;
		ComPtr<ID3D11Device4> device3d;
		ComPtr<ID3D11DeviceContext3> context3d;
		ComPtr<ID2D1Factory3> factory2d;
		ComPtr<ID2D1Device2> device2d;
		ComPtr<ID2D1DeviceContext2> context2d;
	};
}
