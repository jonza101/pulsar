
#include "_gfx/_gfx.h"


PULSAR::gfx::gfx()
{
	WIN_ASSERT(CoInitializeEx(NULL, COINIT_MULTITHREADED));
	GFX_ASSERT(CreateDXGIFactory(IID_IDXGIFactory, (void**)&this->mp_idxgi_factory));

	UINT device_flags = 0u;
	device_flags |= D3D11_CREATE_DEVICE_DEBUG;

	GFX_ASSERT(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, device_flags, NULL, NULL, D3D11_SDK_VERSION, &this->mp_device, NULL, &this->mp_device_context));
}
PULSAR::gfx::~gfx()
{
	this->mp_device->Release();
	this->mp_device_context->Release();
	this->mp_idxgi_factory->Release();
	CoUninitialize();
}

IDXGIFactory	*PULSAR::gfx::idxgi_factory()
{
	return (this->mp_idxgi_factory);
}
ID3D11Device	*PULSAR::gfx::device()
{
	return (this->mp_device);
}
ID3D11DeviceContext		*PULSAR::gfx::device_context()
{
	return (this->mp_device_context);
}
