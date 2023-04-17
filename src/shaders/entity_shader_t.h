#pragma once

#include <GL/gl.h>

#include "shader_t.h"

#include "math/matrix4f_t.h"

class entity_shader_t : public shader_t {
public:
	GLuint shader_view_transform;
	GLuint shader_projection_transform;
	GLuint shader_entity_transform;

	entity_shader_t() noexcept;

	void load_view_transform(const matrix4f_t& matrix) const noexcept;
	void load_projection_transform(const matrix4f_t& matrix) const noexcept;

	void load_entity_transform(const matrix4f_t& matrix) const noexcept;
};
