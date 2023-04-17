#pragma once

#include <GL/gl.h>

#include "math/matrix4f_t.h"

class shader_t {
public:
	GLuint program_id;

private:
	void set_shader_source_and_compile(GLuint shader_id, const char *source_debug_id, const char *source) noexcept;

protected:
	shader_t(const char *vertex_source_debug_id, const char *vertex_source, const char *fragment_source_debug_id, const char *fragment_source) noexcept;

	void load_matrix4f(GLuint uniform_id, const matrix4f_t& matrix) const noexcept;
};
