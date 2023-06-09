#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "skybox_shader_t.h"

#include "math/matrix4f_t.h"

const char *skybox_vertex_shader = R"unique_scope(
#version 330 core			// TODO: add CPU check to see if this shader version is even supported, or maybe just leave it to the info log?

layout(location = 0) in vec3 vertex_position_modelspace;

out vec3 vertex_texture_coords;

uniform mat4 shader_view_transform;
uniform mat4 shader_projection_transform;

void main() {
    vertex_texture_coords = vertex_position_modelspace;
    vec4 temp_frag_pos_4d = shader_view_transform * vec4(vertex_position_modelspace, 1);
    gl_Position = (shader_projection_transform * temp_frag_pos_4d).xyww;
    // NOTE: The xyww means to replace z with w and leave w untouched. When perspective divide is done, w / w will be the result.
    // As a result, the vertex will be at depth 1 in the NDC space thing AFAIK, and that'll always be all the way at the back of the
    // scene. Requires GL_LEQUAL as depth func to work.
}
)unique_scope";

const char *skybox_fragment_shader = R"unique_scope(
#version 330 core

in vec3 vertex_texture_coords;

out vec4 color;

uniform samplerCube cube_map_sampler;

void main() {
    color = texture(cube_map_sampler, vertex_texture_coords);
}
)unique_scope";

skybox_shader_t::skybox_shader_t() noexcept : shader_t("skybox_vertex_shader",   skybox_vertex_shader, 
						       "skybox_fragment_shader", skybox_fragment_shader) {

	shader_view_transform       = glGetUniformLocation(program_id, "shader_view_transform");
	shader_projection_transform = glGetUniformLocation(program_id, "shader_projection_transform");

}

void skybox_shader_t::load_view_transform(const matrix4f_t& matrix) const noexcept {
	glUseProgram(program_id);
	load_matrix4f(shader_view_transform, matrix);
}
void skybox_shader_t::load_projection_transform(const matrix4f_t& matrix) const noexcept {
	glUseProgram(program_id);
	load_matrix4f(shader_projection_transform, matrix);
}
