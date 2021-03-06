
#include "mesh.h"
#include "utils/utils.h"
#include <iostream>
#include <fstream>
#include <string>


bool	pulsar::mesh::load_mesh_obj(LPCWSTR file)
{
	std::vector<float> verts;
	std::vector<float> normals;
	std::vector<float> uvs;

	std::ifstream is(file);
	if (!is)
		return (false);

	std::string line;
	while (std::getline(is, line, '\n'))
	{
		std::vector<std::string> split = pulsar::utils::str_split(line.c_str(), ' ');
		if (!split.size())
			continue;

		if (split[0] == "v")
		{
			float vx = std::stof(split[1]);
			float vy = std::stof(split[2]);
			float vz = std::stof(split[3]);

			verts.push_back(vx);
			verts.push_back(vy);
			verts.push_back(vz);

			continue;
		}
		if (split[0] == "vt")
		{
			float uv_x = std::stof(split[1]);
			float uv_y = 1.0f - std::stof(split[2]);

			uvs.push_back(uv_x);
			uvs.push_back(uv_y);

			continue;
		}
		if (split[0] == "vn")
		{
			float nx = std::stof(split[1]);
			float ny = std::stof(split[2]);
			float nz = std::stof(split[3]);

			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			continue;
		}
		if (split[0] == "f")
		{
			int i = -1;
			while (++i < 3)
			{
				std::vector<std::string> vert_info = pulsar::utils::str_split(split[(UINT64)i + 1].c_str(), '/');

				int vert_id = std::stoi(vert_info[0]) - 1;
				pulsar::mesh::mesh_data::vert_data vert;
				vert.pos[0] = verts[(UINT64)vert_id * 3 + 0];
				vert.pos[1] = verts[(UINT64)vert_id * 3 + 1];
				vert.pos[2] = verts[(UINT64)vert_id * 3 + 2];

				if (vert_info.size() >= 2 && !vert_info[1].empty())
				{
					this->m_mesh_data.m_mesh_specs.uv = true;
					int uv_id = std::stoi(vert_info[1]) - 1;

					vert.uv[0] = uvs[(UINT64)uv_id * 2 + 0];
					vert.uv[1] = uvs[(UINT64)uv_id * 2 + 1];
				}
				if (vert_info.size() > 2)
				{
					this->m_mesh_data.m_mesh_specs.n = true;
					int normal_id = std::stoi(vert_info[2]) - 1;

					vert.normal[0] = normals[(UINT64)normal_id * 3 + 0];
					vert.normal[1] = normals[(UINT64)normal_id * 3 + 1];
					vert.normal[2] = normals[(UINT64)normal_id * 3 + 2];
				}

				this->m_mesh_data.m_verts.push_back(vert);
				this->m_mesh_data.m_indices.push_back((UINT32)this->m_mesh_data.m_indices.size());
			}
		}
	}

	verts.clear();
	normals.clear();
	uvs.clear();

	return (true);
}
