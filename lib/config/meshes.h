#pragma once

#include "mesh/mesh.h"


namespace PULSAR
{
	const LPCWSTR STD_CUBE_OBJ_PATH = L"resources/std/cube.obj";

	const LPCWSTR STD_SPHERE16_OBJ_PATH = L"resources/std/sphere16.obj";
	const LPCWSTR STD_SPHERE32_OBJ_PATH = L"resources/std/sphere32.obj";
	const LPCWSTR STD_SPHERE64_OBJ_PATH = L"resources/std/sphere64.obj";


	const PULSAR::mesh STD_CUBE_MESH;// = PULSAR::mesh(STD_CUBE_OBJ_PATH, PULSAR::MESH_FILE_FORMAT::OBJ);
	
	const PULSAR::mesh STD_SPHERE16_MESH;// = PULSAR::mesh(STD_SPHERE16_OBJ_PATH, PULSAR::MESH_FILE_FORMAT::OBJ);
	const PULSAR::mesh STD_SPHERE32_MESH;// = PULSAR::mesh(STD_SPHERE32_OBJ_PATH, PULSAR::MESH_FILE_FORMAT::OBJ);
	const PULSAR::mesh STD_SPHERE64_MESH;// = PULSAR::mesh(STD_SPHERE64_OBJ_PATH, PULSAR::MESH_FILE_FORMAT::OBJ);
}
