#pragma once

#define GL_GLEXT_PROTOTYPES	// NOTE: We define this in every header that uses gl.h because or else
				// conflicts can arise that negate the target behavior.
#include <GL/gl.h>

#include "geometry/mesh_t.h"
#include "texturing/texture_t.h"

#include "math/matrix4f_t.h"

struct entity_t {
	const mesh_t *mesh;
	const texture_t *texture;

	matrix4f_t transform;
};
