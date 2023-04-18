#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "entity_shader_t.h"

#include "math/matrix4f_t.h"

const char *entity_vertex_shader = R"unique_scope(
#version 330 core			// TODO: add CPU check to see if this shader version is even supported, or maybe just leave it to the info log?

layout(location = 0) in vec3 vertex_position_modelspace;

uniform mat4 shader_view_transform;
uniform mat4 shader_projection_transform;
uniform mat4 shader_entity_transform;

void main() {
    vec4 temp_frag_pos_4d = shader_view_transform * shader_entity_transform * vec4(vertex_position_modelspace, 1);
    gl_Position = shader_projection_transform * temp_frag_pos_4d;
}
)unique_scope";

const char *entity_fragment_shader = R"unique_scope(
#version 330 core

out vec3 color;

void main() {
    color = vec3(1, 1, 1);
}
)unique_scope";

entity_shader_t::entity_shader_t() noexcept : shader_t("entity_vertex_shader", entity_vertex_shader, 
						       "entity_fragment_shader", entity_fragment_shader) {

	shader_view_transform       = glGetUniformLocation(program_id, "shader_view_transform");
	shader_projection_transform = glGetUniformLocation(program_id, "shader_projection_transform");
	shader_entity_transform     = glGetUniformLocation(program_id, "shader_entity_transform");

}

void entity_shader_t::load_view_transform(const matrix4f_t& matrix) const noexcept {
	glUseProgram(program_id);
	load_matrix4f(shader_view_transform, matrix);
}
void entity_shader_t::load_projection_transform(const matrix4f_t& matrix) const noexcept {
	glUseProgram(program_id);
	load_matrix4f(shader_projection_transform, matrix);
}
void entity_shader_t::load_entity_transform(const matrix4f_t& matrix) const noexcept {
	glUseProgram(program_id);
	load_matrix4f(shader_entity_transform, matrix);
}
