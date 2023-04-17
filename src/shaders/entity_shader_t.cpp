#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "entity_shader_t.h"

#include "math/matrix4f_t.h"

entity_shader_t::entity_shader_t() noexcept : shader_t("entity_vertex_shader", "TODO: implement", "entity_fragment_shader", "TODO: implement") {
	shader_view_transform       = glGetUniformLocation(program_id, "shader_view_transform");
	shader_projection_transform = glGetUniformLocation(program_id, "shader_projection_transform");
	shader_entity_transform     = glGetUniformLocation(program_id, "shader_entity_transform");
}

void entity_shader_t::load_view_transform(const matrix4f_t& matrix) const noexcept       { load_matrix4f(shader_view_transform, matrix); }
void entity_shader_t::load_projection_transform(const matrix4f_t& matrix) const noexcept { load_matrix4f(shader_projection_transform, matrix); }
void entity_shader_t::load_entity_transform(const matrix4f_t& matrix) const noexcept     { load_matrix4f(shader_entity_transform, matrix); }
