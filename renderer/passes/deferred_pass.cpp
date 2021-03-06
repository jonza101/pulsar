
#include "deferred_pass.h"
#include "config/config.h"
#include "config/shader.h"
#include "renderer/renderer.h"
#include "config/rasterizer_state.h"
#include "shaders/frag_shader.h"
#include "depth_stencil/depth_stencil_view.h"
#include "viewport/viewport.h"
#include "render_texture/render_texture.h"
#include "sampler/sampler.h"
#include "scene/scene.h"


pulsar::deferred_pass::deferred_pass(const std::string &name) : pulsar::fullscreen_pass(name)
{
	this->mp_deferred_fs = new pulsar::frag_shader(pulsar::DEFERRED_FS_PATH);
	this->mp_sampler = new pulsar::sampler(pulsar::DEFERRED_FRAG_SAMPLER_SLOT);

	this->mp_hdr_buffer_input = new pulsar::rg::buffer_input<pulsar::render_texture>(pulsar::RG_G_HDR_BUFFER, this->mp_hdr_buffer);
	this->mp_hdr_buffer_source = new pulsar::rg::buffer_source<pulsar::render_texture>(pulsar::RG_G_HDR_BUFFER, this->mp_hdr_buffer);

	this->mp_ds_view_input = new pulsar::rg::buffer_input<pulsar::depth_stencil_view>(pulsar::RG_G_DS_VIEW, this->mp_ds_view);
	this->mp_ds_view_source = new pulsar::rg::buffer_source<pulsar::depth_stencil_view>(pulsar::RG_G_DS_VIEW, this->mp_ds_view);

	this->register_input(this->mp_hdr_buffer_input);
	this->register_source(this->mp_hdr_buffer_source);

	this->register_input(this->mp_ds_view_input);
	this->register_source(this->mp_ds_view_source);

	int i = -1;
	while (++i < pulsar::G_BUFFERS_COUNT)
	{
		this->mp_g_buffers_inputs[i] = new pulsar::rg::buffer_input<pulsar::render_texture>(pulsar::RG_G_G_BUFFERS[i], this->mp_g_buffers[i]);
		this->mp_g_buffers_sources[i] = new pulsar::rg::buffer_source<pulsar::render_texture>(pulsar::RG_G_G_BUFFERS[i], this->mp_g_buffers[i]);

		this->register_input(this->mp_g_buffers_inputs[i]);
		this->register_source(this->mp_g_buffers_sources[i]);
	}
}

pulsar::deferred_pass::~deferred_pass()
{
	delete this->mp_deferred_fs;
	delete this->mp_sampler;

	delete this->mp_hdr_buffer_input;
	delete this->mp_hdr_buffer_source;

	delete this->mp_ds_view_input;
	delete this->mp_ds_view_source;

	int i = -1;
	while (++i < pulsar::G_BUFFERS_COUNT)
	{
		delete this->mp_g_buffers_inputs[i];
		delete this->mp_g_buffers_sources[i];
	}
}

void	pulsar::deferred_pass::validate() const
{
	if (!this->mp_ds_view)
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

void	pulsar::deferred_pass::execute()
{
	pulsar::renderer &renderer = pulsar::renderer::instance();
	pulsar::scene *active_scene = pulsar::scene::get_active_scene();

	if (!active_scene || !active_scene->get_main_camera())
		return;

	auto *camera_viewport = renderer.get_main_camera_viewport();
	auto *deferred_frag_lights_cbuffer = renderer.get_deferred_frag_lights_cbuffer();
	auto *frag_camera_cbuffer = renderer.get_frag_camera_cbuffer();

	{
		pulsar::BACK_FACE_CULL_RS.bind();
		this->mp_deferred_fs->bind();
		this->mp_hdr_buffer->bind_rtv();
		this->mp_sampler->bind();
		camera_viewport->bind();

		deferred_frag_lights_cbuffer->bind();
		frag_camera_cbuffer->set_slot(pulsar::DEFERRED_FRAG_CAMERA_SLOT);
		frag_camera_cbuffer->bind();

		int i = -1;
		while (++i < pulsar::G_BUFFERS_COUNT)
			this->mp_g_buffers[i]->bind_srv();
	}

	pulsar::fullscreen_pass::execute();

	{
		int i = pulsar::G_BUFFERS_COUNT;
		while (--i >= 0)
			this->mp_g_buffers[i]->unbind_srv();

		frag_camera_cbuffer->unbind();
		deferred_frag_lights_cbuffer->unbind();

		camera_viewport->unbind();
		this->mp_sampler->unbind();
		this->mp_hdr_buffer->unbind_rtv();
		this->mp_deferred_fs->unbind();
		pulsar::BACK_FACE_CULL_RS.unbind();
	}
}
