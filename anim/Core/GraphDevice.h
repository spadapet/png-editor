#pragma once

#include "Core/Event.h"
#include "Core/GraphContextLock.h"

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
		GraphContextLock3d GetContext3d();
		GraphContextLock3d TryGetContext3d();
		ID2D1Factory3 *GetFactory2d() const;
		ID2D1Device2 *GetDevice2d() const;
		GraphContextLock2d GetContext2d();
		GraphContextLock2d TryGetContext2d();

		void RegisterVertexShader(const std::string &name, std::vector<D3D11_INPUT_ELEMENT_DESC> layoutElements);
		void RegisterPixelShader(const std::string &name);
		ID3D11VertexShader *GetVertexShader(const std::string &name);
		ID3D11InputLayout *GetVertexShaderInputLayout(const std::string &name);
		ID3D11PixelShader *GetPixelShader(const std::string &name);

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

		CRITICAL_SECTION lockContext3d;
		CRITICAL_SECTION lockContext2d;
	};
}
