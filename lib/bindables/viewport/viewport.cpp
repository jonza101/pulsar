
#include "viewport.h"


PULSAR::viewport::viewport(UINT width, UINT height, float min_depth, float max_depth)
{
	this->create_viewport(width, height, min_depth, max_depth);
}

void	PULSAR::viewport::resize(UINT width, UINT height)
{
	this->m_viewport.Width = width;
	this->m_viewport.Height = height;

	this->resize_linked(width, height);
}

void	PULSAR::viewport::clear()
{

}

void	PULSAR::viewport::bind() const
{
	PULSAR::gfx::get().device_context()->RSSetViewports(1u, &this->m_viewport);
}

void	PULSAR::viewport::unbind() const
{
	D3D11_VIEWPORT null_ptr[1] = { NULL };
	PULSAR::gfx::get().device_context()->RSSetViewports(1u, null_ptr);
}
