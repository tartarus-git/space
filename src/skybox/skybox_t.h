#pragma once

#include "texturing/skybox_texture_t.h"

struct skybox_t {
	const mesh_t *mesh;
	const skybox_texture_t *texture;
};
