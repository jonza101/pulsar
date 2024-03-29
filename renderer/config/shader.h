#pragma once

#include "config.h"
#include "gfx_resources/vert_shader.h"
#include "gfx_resources/frag_shader.h"


namespace pulsar
{
	const pulsar::vert_shader FULLSCREEN_VS(pulsar::FULLSCREEN_VS_PATH);
	const pulsar::frag_shader OPAQUE_GEOMETRY_G_BUFFER_FS(pulsar::OPAQUE_G_BUFFER_FS_PATH);
}