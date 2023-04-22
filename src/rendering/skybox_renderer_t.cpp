#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "renderer_t.h"
#include "skybox_renderer_t.h"

#include "scene/scene_t.h"
#include "rendering/camera_t.h"
#include "math/matrix4f_t.h"

skybox_renderer_t::skybox_renderer_t(const scene_t *scene, 
				     const camera_t& camera, 
				     const matrix4f_t *projection_matrix) noexcept : 
					renderer_t(scene), 
					view_transform(matrix4f_t::gen_fps_rotation(-camera.rotation)), 
					projection_transform(projection_matrix) { }

void skybox_renderer_t::overwrite_view_transform_with_camera(const camera_t& camera) noexcept {
	view_transform = matrix4f_t::gen_fps_rotation(-camera.rotation);
}

void skybox_renderer_t::apply_matrix_to_view_transform(const matrix4f_t& matrix) noexcept {
	view_transform = matrix * view_transform;
}

void skybox_renderer_t::update_projection_transform(const matrix4f_t *matrix_ptr) noexcept {
	projection_transform = matrix_ptr;
}

void skybox_renderer_t::render() noexcept {
	glUseProgram(scene->skybox_shader->program_id);

	scene->skybox_shader->load_view_transform(view_transform);
	scene->skybox_shader->load_projection_transform(*projection_transform);
	// TODO: Probs don't do the above two every render.
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene->skybox.texture->id);

	glBindVertexArray(scene->skybox.mesh->VAO_id);
	glEnableVertexAttribArray(0);
	// TODO: Consider switching to SHORT, but it'll require a different vector. You could template the mesh class.
	glDrawElements(GL_TRIANGLES, scene->skybox.mesh->indices.size() * 3, GL_UNSIGNED_INT, nullptr);
	// NOTE: Not disabling for now because it's pointless.
}
