
//#define NOMINMAX

#include "pulsar.h"

using namespace PULSAR;

#include "cam_controller.h"

#include "ecs/registry.h"
#include "node/node.h"
#include "containers/sparse_set.h"


int CALLBACK	WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show)
{
	/*{
		struct transform_comp
		{
			int a = 1;

			transform_comp() = default;
			transform_comp(int a) : a(a)
			{
			}
		};
		struct mesh_comp
		{
			int b = 2;
		};
		struct mat_comp
		{
			int c = 3;
		};


		{
			PULSAR::sparse_set<uint32_t> s_set;
			s_set.insert(0u);
			s_set.insert(5u);
			s_set.insert(3u);
			s_set.insert(1u);
			s_set.insert(10u);

			s_set.erase(5u);

			for (auto v : s_set)
			{
				uint32_t a = v;
			}


			int f = 1;
		}



		PULSAR::ecs::registry reg;

		auto id0 = reg.create();
		auto id1 = reg.create();
		auto id2 = reg.create();
		auto id3 = reg.create();
		auto id4 = reg.create();
		auto id5 = reg.create();


		reg.emplace<transform_comp>(id0);
		reg.emplace<mesh_comp>(id0);

		reg.emplace<transform_comp>(id1, 42);
		reg.emplace<mat_comp>(id1);
		reg.emplace<mesh_comp>(id1);

		reg.emplace<transform_comp>(id2, 41);
		reg.emplace<transform_comp>(id4, 43);

		bool id0_mat_e = reg.has<mat_comp>(id0);
		bool id1_trans_e = reg.has<transform_comp>(id1);

		reg.get<mat_comp>(id1).c = 21;

		const PULSAR::ecs::group trans_mesh_group = reg.group<transform_comp, mesh_comp>();
		const PULSAR::ecs::group mesh_mat_group = reg.group<mesh_comp, mat_comp>();
		const PULSAR::ecs::group trans_group = reg.group<transform_comp>();
		const PULSAR::ecs::group mesh_group = reg.group<mesh_comp>();
		const PULSAR::ecs::group empty_group = reg.group<int>();


		reg.erase<transform_comp>(id1);
		reg.erase<mesh_comp>(id1);
		reg.erase<mat_comp>(id1);

		id1_trans_e = reg.has<transform_comp>(id1);

		reg.destroy(id0);

		int a = 1;
	}*/
	exit(0);



	//PULSAR_D3D::init();
	PULSAR::init(h_instance);

	PULSAR::window::get().set("PULSAR", 1280, 720);

	CAMERA *camera = CAMERA::create();
	TRANSFORM cam_transform = TRANSFORM(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f));
	camera->set(cam_transform, deg2rad(75.0f));
	CAM_CONTROLLER_DATA cam_controller_data{ 5.0f, 6.0f, 0.8f, 0.75f, { cam_transform.rotation.x, cam_transform.rotation.y } };
	win->set_camera(camera);

	win->add_update(cam_controller, &cam_controller_data);


	scene *scene = scene::create();
	win->set_scene(scene);

	SKYBOX_MATERIAL *skybox_mat = SKYBOX_MATERIAL::create();
	skybox_mat->env_map.set(L"resources/cubemaps/cm00/cm00.dds");
	scene->set_skybox_material(skybox_mat);


	vert_shader gb_pass_vs(L"shaders/g_buffer/g_buffer_pass_vs.hlsl");
	geom_shader gb_pass_gs(L"shaders/g_buffer/g_buffer_pass_gs.hlsl");
	frag_shader gb_pass_fs(L"shaders/g_buffer/g_buffer_pass_fs.hlsl");


	OBJECT *cube_obj = OBJECT::create();
	OBJECT *sph_obj = OBJECT::create();
	OBJECT *sph1_obj = OBJECT::create();
	OBJECT *sph2_obj = OBJECT::create();
	scene->add_entity(cube_obj);
	scene->add_entity(sph_obj);
	scene->add_entity(sph1_obj);
	scene->add_entity(sph2_obj);


	cube_obj->set_mesh(STD_CUBE_MESH);

	MATERIAL *cube_mat = MATERIAL::create();
	cube_mat->albedo_map().set(L"resources/textures/p3.jpg");
	cube_mat->normal_map().set(L"resources/textures/brick00/normal.png");
	cube_mat->normal_factor() = 1.0f;
	cube_obj->set_material(cube_mat);


	sph_obj->set_mesh(STD_SPHERE64_MESH);

	MATERIAL *sph_mat = MATERIAL::create();
	sph_mat->albedo_map().set(L"resources/textures/brick00/albedo.png");
	sph_mat->normal_map().set(L"resources/textures/brick00/normal.png");
	sph_mat->normal_factor() = 1.5f;
	sph_mat->roughness_map().set(L"resources/textures/brick00/roughness.png");
	sph_mat->metalness() = 0.0f;
	sph_mat->ao() = 0.01f;
	sph_obj->set_material(sph_mat);


	sph1_obj->set_mesh(STD_SPHERE64_MESH);

	MATERIAL *sph1_mat = MATERIAL::create();
	sph1_mat->albedo_map().set(L"resources/textures/metal00/albedo.png");
	sph1_mat->normal_map().set(L"resources/textures/metal00/normal.png");
	sph1_mat->normal_factor() = 5.0f;
	sph1_mat->roughness_map().set(L"resources/textures/metal00/roughness.png");
	sph1_mat->metalness_map().set(L"resources/textures/metal00/metalness.png");
	sph1_mat->ao() = 0.01f;
	sph1_obj->set_material(sph1_mat);


	sph2_obj->set_mesh(STD_SPHERE64_MESH);

	MATERIAL *sph2_mat = MATERIAL::create();
	sph2_mat->albedo_map().set(L"resources/textures/wood00/albedo.png");
	sph2_mat->normal_map().set(L"resources/textures/wood00/normal.png");
	sph2_mat->normal_factor() = 4.0f;
	sph2_mat->roughness_map().set(L"resources/textures/wood00/roughness.png");
	sph2_mat->metalness_map().set(L"resources/textures/wood00/metalness.png");
	sph2_mat->ao() = 0.01f;
	sph2_obj->set_material(sph2_mat);


	POINT_LIGHT *pl1 = POINT_LIGHT::create();
	DIR_LIGHT *dl1 = DIR_LIGHT::create();
	scene->add_light(pl1);
	scene->add_light(dl1);

	pl1->set(XMFLOAT3(-5.0f, -2.0f, -1.0f), XMFLOAT3(0.15f, 0.15f, 1.0f), 1.0f, 0.005f, 0.0007f);
	dl1->set(XMFLOAT3(0.0f, -0.25f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.5f));


	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	input_layout input_layout(gb_pass_vs.get_shader_blob(), ied, (UINT)std::size(ied));


	cube_obj->add_bindable(gb_pass_vs);
	cube_obj->add_bindable(gb_pass_gs);
	cube_obj->add_bindable(gb_pass_fs);
	cube_obj->add_bindable(input_layout);

	sph_obj->add_bindable(gb_pass_vs);
	sph_obj->add_bindable(gb_pass_gs);
	sph_obj->add_bindable(gb_pass_fs);
	sph_obj->add_bindable(input_layout);

	sph1_obj->add_bindable(gb_pass_vs);
	sph1_obj->add_bindable(gb_pass_gs);
	sph1_obj->add_bindable(gb_pass_fs);
	sph1_obj->add_bindable(input_layout);

	sph2_obj->add_bindable(gb_pass_vs);
	sph2_obj->add_bindable(gb_pass_gs);
	sph2_obj->add_bindable(gb_pass_fs);
	sph2_obj->add_bindable(input_layout);


	cube_obj->transform.position = XMFLOAT3(1.0f, 0.0f, 5.5f);
	cube_obj->transform.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	sph_obj->transform.position = XMFLOAT3(0.0f, 0.0f, 1.75f);
	sph_obj->transform.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	sph1_obj->transform.position = XMFLOAT3(1.25f, 0.0f, 1.75f);
	sph1_obj->transform.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	sph2_obj->transform.position = XMFLOAT3(-1.25f, 0.0f, 1.75f);
	sph2_obj->transform.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	while (WINDOW::process_events())
	{
		for (auto &it : PULSAR::WIN_MANAGER::get_instance())
		{
			WINDOW *win = it.data;

			float delta_time = GFX::get_delta_time();
			cube_obj->transform.rotation = XMFLOAT3(cube_obj->transform.rotation.x + delta_time * 0.25f, 0.0f, cube_obj->transform.rotation.z + delta_time * 0.25f);
			sph_obj->transform.rotation = XMFLOAT3(sph_obj->transform.rotation.x + delta_time * 0.25f, sph_obj->transform.rotation.y + delta_time * 0.25f, sph_obj->transform.rotation.z + delta_time * 0.25f);
			sph1_obj->transform.rotation = XMFLOAT3(sph1_obj->transform.rotation.x + delta_time * 0.25f, sph1_obj->transform.rotation.y + delta_time * 0.25f, sph1_obj->transform.rotation.z + delta_time * 0.25f);
			sph2_obj->transform.rotation = XMFLOAT3(sph2_obj->transform.rotation.x + delta_time * 0.25f, sph2_obj->transform.rotation.y + delta_time * 0.25f, sph2_obj->transform.rotation.z + delta_time * 0.25f);

			win->update();
			win->draw();

			win->present();
		}
		GFX::set_delta_time();
	}

	RESOURCE_MANAGER::terminate();
	PULSAR::terminate();

	return (0);
}
