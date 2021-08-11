#pragma once

#include "grng_rasterizer_state.h"


const GRNG_RASTERIZER_STATE GRNG_STD_BACK_FACE_CULL_RS = GRNG_RASTERIZER_STATE(D3D11_FILL_SOLID, D3D11_CULL_BACK);
const GRNG_RASTERIZER_STATE GRNG_STD_FRONT_FACE_CULL_RS = GRNG_RASTERIZER_STATE(D3D11_FILL_SOLID, D3D11_CULL_FRONT);