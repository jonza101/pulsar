#pragma once

#include "grng_id3d.h"

#include <unordered_map>


enum class GRNG_BINDABLE_TYPE
{
	NONE,
	VERT_CONST_BUFFER,
	FRAG_CONST_BUFFER,
	TEXTURE,
	DEPTH_STENCIL_VIEW,
	DEPTH_STENCIL_STATE,
	INPUT_LAYOUT,
	RENDER_TEXTURE,
	SAMPLER,
	VERT_SHADER,
	GEOM_SHADER,
	FRAG_SHADER,
	CUBEMAP,
	RASTERIZER_STATE
};


class grng_bindable : public GRNG_ID3D
{
private:
	friend class grng_bindable_manager;
	friend class grng_entity;

protected:
	GRNG_BINDABLE_TYPE		type = GRNG_BINDABLE_TYPE::NONE;
	int						id = -1;

	std::unordered_map<int, int>	entity_scene_map;		//	ENTITY_ID TO SCENE_ID

public:
	grng_bindable() : GRNG_ID3D(){ }

	GRNG_BINDABLE_TYPE		get_type() const
	{
		return (this->type);
	}

	int						get_id() const
	{
		return (this->id);
	}

	virtual void		bind() const = 0;
};

using GRNG_BINDABLE = grng_bindable;
