#pragma once

#include "geometry/mesh_t.h"
#include "texturing/texture_t.h"

#include "math/matrix4f_t.h"

struct entity_t {
	const mesh_t *mesh;
	const texture_t *texture;

	matrix4f_t transform;
};
