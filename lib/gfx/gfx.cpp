
#include "gfx.h"
#include "exceptions/win_exception.h"
#include "exceptions/gfx_exception.h"


pulsar::gfx::gfx()
{
	WIN_ASSERT(CoInitializeEx(NULL, COINIT_MULTITHREADED));
	GFX_ASSERT(CreateDXGIFactory(IID_IDXGIFactory, (void**)&this->mp_idxgi_factory));

	UINT device_flags = 0u;
	device_flags |= D3D11_CREATE_DEVICE_DEBUG;

	GFX_ASSERT(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, device_flags, NULL, NULL, D3D11_SDK_VERSION, &this->mp_device, NULL, &this->mp_device_context));
}

pulsar::gfx::~gfx()
{
	if (this->mp_device)
		this->mp_device->Release();
	if (this->mp_device_context)
		this->mp_device_context->Release();
	if (this->mp_idxgi_factory)
		this->mp_idxgi_factory->Release();
	CoUninitialize();
}

IDXGIFactory	*pulsar::gfx::idxgi_factory()
{
	return (this->mp_idxgi_factory);
}

ID3D11Device	*pulsar::gfx::device()
{
	return (this->mp_device);
}

ID3D11DeviceContext		*pulsar::gfx::device_context()
{
	return (this->mp_device_context);
}

void	pulsar::gfx::draw_indexed(UINT index_count) const
{
	this->mp_device_context->DrawIndexed(index_count, 0u, 0u);
}
