#include "pch.h"
#include "Core/GraphDevice.h"

anim::GraphDevice::GraphDevice()
{
}

anim::GraphDevice::~GraphDevice()
{
}

ComPtr<IDXGIFactory5> anim::GraphDevice::GetFactory() const
{
	return this->factory;
}

ComPtr<ID3D11Device4> anim::GraphDevice::GetDevice() const
{
	return this->device;
}
