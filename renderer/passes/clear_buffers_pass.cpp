
#include "clear_buffers_pass.h"
#include "config/config.h"
#include "exceptions/rg_exception.h"
#include "gfx_resources/framebuffer.h"
#include "gfx_resources/depth_stencil_view.h"
#include "gfx_resources/render_texture.h"


pulsar::clear_buffers_pass::clear_buffers_pass(const std::string &name) : pulsar::rg::pass(name)
{
	this->mp_framebuffer_input = new pulsar::rg::sync_input<pulsar::framebuffer>(pulsar::RG_G_FRAMEBUFFER, this->mp_framebuffer);
	this->mp_frambuffer_source = new pulsar::rg::sync_source<pulsar::framebuffer>(pulsar::RG_G_FRAMEBUFFER, this->mp_framebuffer);

	this->mp_dsv_input = new pulsar::rg::sync_input<pulsar::depth_stencil_view>(pulsar::RG_G_DSV, this->mp_dsv);
	this->mp_dsv_source = new pulsar::rg::sync_source<pulsar::depth_stencil_view>(pulsar::RG_G_DSV, this->mp_dsv);

	this->mp_hdr_buffer_input = new pulsar::rg::sync_input<pulsar::render_texture>(pulsar::RG_G_HDR_BUFFER, this->mp_hdr_buffer);
	this->mp_hdr_buffer_source = new pulsar::rg::sync_source<pulsar::render_texture>(pulsar::RG_G_HDR_BUFFER, this->mp_hdr_buffer);

	this->register_input(this->mp_framebuffer_input);
	this->register_source(this->mp_frambuffer_source);

	this->register_input(this->mp_dsv_input);
	this->register_source(this->mp_dsv_source);

	this->register_input(this->mp_hdr_buffer_input);
	this->register_source(this->mp_hdr_buffer_source);

	int i = -1;
	while (++i < pulsar::G_BUFFERS_COUNT)
	{
		this->mp_g_buffers_inputs[i] = new pulsar::rg::sync_input<pulsar::render_texture>(pulsar::RG_G_G_BUFFERS[i], this->mp_g_buffers[i]);
		this->mp_g_buffers_sources[i] = new pulsar::rg::sync_source<pulsar::render_texture>(pulsar::RG_G_G_BUFFERS[i], this->mp_g_buffers[i]);

		this->register_input(this->mp_g_buffers_inputs[i]);
		this->register_source(this->mp_g_buffers_sources[i]);
	}
}

pulsar::clear_buffers_pass::~clear_buffers_pass()
{
	delete this->mp_framebuffer_input;
	delete this->mp_frambuffer_source;

	delete this->mp_dsv_input;
	delete this->mp_dsv_source;

	delete this->mp_hdr_buffer_input;
	delete this->mp_hdr_buffer_source;

	int i = -1;
	while (++i < pulsar::G_BUFFERS_COUNT)
	{
		delete this->mp_g_buffers_inputs[i];
		delete this->mp_g_buffers_sources[i];
	}
}

void	pulsar::clear_buffers_pass::validate() const
{
	if (!this->mp_framebuffer)
		THROW_RG_EXC("Framebuffer isn't bound");
	if (!this->mp_dsv)
		THROW_RG_EXC("Depth Stencil View isn't bound");
	if (!this->mp_hdr_buffer)
		THROW_RG_EXC("HDR Buffer isn't bound");

	int i = -1;
	while (++i < pulsar::G_BUFFERS_COUNT)
	{
		if (!this->mp_g_buffers[i])
			THROW_RG_EXC("G-Buffer isn't bound");
	}
}

void	pulsar::clear_buffers_pass::execute()
{
	this->mp_framebuffer->clear();
	this->mp_dsv->clear();
	this->mp_hdr_buffer->clear();

	int i = -1;
	while (++i < pulsar::G_BUFFERS_COUNT)
		this->mp_g_buffers[i]->clear();
}
