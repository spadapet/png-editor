#include "pch.h"
#include "Core/GraphDevice.h"

anim::GraphDevice::GraphDevice()
	: valid(false)
{
}

anim::GraphDevice::~GraphDevice()
{
}

void anim::GraphDevice::Initialize()
{
	assert(!this->valid);

	this->valid = this->InternalInitialize();

	if (!this->valid)
	{
		this->Reset(false);
	}
}

bool anim::GraphDevice::IsValid() const
{
	return this->valid;
}

bool anim::GraphDevice::ResetIfNeeded()
{
	if (this->valid && !this->factory->IsCurrent())
	{
		this->Reset(true);
	}

	return this->valid;
}

IDXGIFactory5 *anim::GraphDevice::GetFactory() const
{
	return this->factory.Get();
}

IDXGIDevice4 *anim::GraphDevice::GetDevice() const
{
	return this->device.Get();
}

ID3D11Device4 *anim::GraphDevice::GetDevice3d() const
{
	return this->device3d.Get();
}

ID3D11DeviceContext3 *anim::GraphDevice::GetContext3d() const
{
	return this->context3d.Get();
}

ID2D1Factory3 *anim::GraphDevice::GetFactory2d() const
{
	return this->factory2d.Get();
}

ID2D1Device2 *anim::GraphDevice::GetDevice2d() const
{
	return this->device2d.Get();
}

ID2D1DeviceContext2 *anim::GraphDevice::GetContext2d() const
{
	return this->context2d.Get();
}

bool anim::GraphDevice::InternalInitialize()
{
#ifdef _DEBUG
	UINT factoryFlags = DXGI_CREATE_FACTORY_DEBUG;
	UINT deviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D2D1_DEBUG_LEVEL debugLevel = D2D1_DEBUG_LEVEL_WARNING;
#else
	UINT factoryFlags = 0;
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D2D1_DEBUG_LEVEL debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	if (FAILED(::CreateDXGIFactory2(factoryFlags, __uuidof(IDXGIFactory5), (void **)this->factory.GetAddressOf())))
	{
		return false;
	}

	for (int software = 0; software <= 1; software++)
	{
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		if (SUCCEEDED(::D3D11CreateDevice(
			nullptr,
			software ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			deviceFlags,
			featureLevels,
			_countof(featureLevels),
			D3D11_SDK_VERSION,
			&device,
			nullptr,
			&context)))
		{
			device.As(&this->device);
			device.As(&this->device3d);
			context.As(&this->context3d);
			break;
		}
	}

	if (this->device == nullptr ||
		this->device3d == nullptr ||
		this->context3d == nullptr)
	{
		return false;
	}

	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = debugLevel;

	if (FAILED(::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory3), &options, (void**)this->factory2d.GetAddressOf())))
	{
		return false;
	}

	if (FAILED(this->factory2d->CreateDevice(this->device.Get(), &this->device2d)))
	{
		return false;
	}

	if (FAILED(this->device2d->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &this->context2d)))
	{
		return false;
	}

	return true;
}

void anim::GraphDevice::Reset(bool reinit)
{
	assert(reinit);

	this->valid = false;
	this->context2d.Reset();
	this->device2d.Reset();
	this->factory2d.Reset();
	this->context3d.Reset();
	this->device3d.Reset();
	this->device.Reset();
	this->factory.Reset();

	if (reinit)
	{
		this->Initialize();
		this->DeviceReset.Notify();
	}
}
