
#include "skybox_pass.h"
#include "config/config.h"
#include "config/rasterizer_state.h"
#include "config/mesh.h"
#include "renderer/renderer.h"
#include "mesh/cube.h"
#include "gfx_resources/vert_shader.h"
#include "gfx_resources/frag_shader.h"
#include "gfx_resources/depth_stencil_state.h"
#include "gfx_resources/sampler.h"
#include "gfx_resources/input_layout.h"
#include "gfx_resources/render_texture.h"
#include "gfx_resources/depth_stencil_view.h"
#include "gfx_resources/viewport.h"
#include "scene/scene.h"


pulsar::skybox_pass::skybox_pass(const std::string &name) : pulsar::rg::pass(name)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	this->mp_skybox_mesh = new pulsar::cube;
	this->mp_skybox_vs = new pulsar::vert_shader(pulsar::SKYBOX_VS_PATH);
	this->mp_skybox_fs = new pulsar::frag_shader(pulsar::SKYBOX_FS_PATH);
	this->mp_dss = new pulsar::depth_stencil_state(TRUE, D3D11_COMPARISON_LESS_EQUAL, D3D11_DEPTH_WRITE_MASK_ZERO);
	this->mp_sampler = new pulsar::sampler(pulsar::FRAG_SKYBOX_SAMPLER_SLOT);
	this->mp_input_layout = new pulsar::input_layout(this->mp_skybox_vs->blob(), ied, (UINT)std::size(ied));

	this->mp_hdr_buffer_input = new pulsar::rg::sync_input<pulsar::render_texture>(pulsar::RG_G_HDR_BUFFER, this->mp_hdr_buffer);
	this->mp_hdr_buffer_source = new pulsar::rg::sync_source<pulsar::render_texture>(pulsar::RG_G_HDR_BUFFER, this->mp_hdr_buffer);

	this->mp_dsv_input = new pulsar::rg::sync_input<pulsar::depth_stencil_view>(pulsar::RG_G_DSV, this->mp_dsv);
	this->mp_dsv_source = new pulsar::rg::sync_source<pulsar::depth_stencil_view>(pulsar::RG_G_DSV, this->mp_dsv);

	this->register_input(this->mp_hdr_buffer_input);
	this->register_source(this->mp_hdr_buffer_source);

	this->register_input(this->mp_dsv_input);
	this->register_source(this->mp_dsv_source);
}

pulsar::skybox_pass::~skybox_pass()
{
	delete this->mp_skybox_mesh;
	delete this->mp_skybox_vs;
	delete this->mp_skybox_fs;
	delete this->mp_dss;
	delete this->mp_sampler;
	delete this->mp_input_layout;

	delete this->mp_hdr_buffer_input;
	delete this->mp_hdr_buffer_source;

	delete this->mp_dsv_input;
	delete this->mp_dsv_source;
}

void	pulsar::skybox_pass::validate() const
{
	if (!this->mp_hdr_buffer)
		THROW_RG_EXC("HDR Buffer isn't bound");
	if (!this->mp_dsv)
		THROW_RG_EXC("Depth Stencil View isn't bound");
}

void	pulsar::skybox_pass::execute()
{
	pulsar::renderer &renderer = pulsar::renderer::instance();
	pulsar::scene *active_scene = pulsar::scene::get_active_scene();

	if (!active_scene || !active_scene->get_main_camera())
		return;

	auto *camera_viewport = renderer.get_main_camera_viewport();
	auto *vert_camera_cbuffer = renderer.get_vert_camera_cbuffer();
	auto *frag_camera_cbuffer = renderer.get_frag_camera_cbuffer();
	auto &skybox_material = active_scene->skybox_material();

	{
		pulsar::FRONT_FACE_CULL_RS.bind();
		this->mp_skybox_mesh->bind();
		this->mp_skybox_vs->bind();
		this->mp_skybox_fs->bind();
		this->mp_dss->bind();
		this->mp_sampler->bind();
		this->mp_input_layout->bind();
		camera_viewport->bind();
		this->mp_hdr_buffer->bind_rtv(this->mp_dsv->dsv());

		vert_camera_cbuffer->set_slot(pulsar::VERT_SKYBOX_CAMERA_SLOT);
		vert_camera_cbuffer->bind();
		skybox_material.bind();
		frag_camera_cbuffer->set_slot(pulsar::FRAG_SKYBOX_CAMERA_SLOT);
		frag_camera_cbuffer->bind();
	}

	pulsar::gfx::instance().draw_indexed(this->mp_skybox_mesh->get_index_count());

	{
		frag_camera_cbuffer->unbind();
		skybox_material.unbind();
		vert_camera_cbuffer->unbind();

		this->mp_hdr_buffer->unbind_rtv();
		camera_viewport->unbind();
		this->mp_input_layout->unbind();
		this->mp_sampler->unbind();
		this->mp_dss->unbind();
		this->mp_skybox_fs->unbind();
		this->mp_skybox_vs->bind();
		this->mp_skybox_mesh->unbind();
		pulsar::FRONT_FACE_CULL_RS.unbind();
	}
}
