#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "renderer_t.h"
#include "entity_renderer_t.h"

#include "scene/scene_t.h"

entity_renderer_t::entity_renderer_t(const scene_t *scene, 
				     const camera_t& camera, 
				     const matrix4f_t *projection_matrix) noexcept : 
					renderer_t(scene), 
					view_transform(camera.gen_view_transform()), 
					projection_transform(projection_matrix) { }

void entity_renderer_t::overwrite_view_transform_with_camera(const camera_t& camera) noexcept {
	view_transform = camera.gen_view_transform();
}

void entity_renderer_t::apply_matrix_to_view_transform(const matrix4f_t& matrix) noexcept {
	view_transform = matrix * view_transform;
}

void entity_renderer_t::update_projection_transform(const matrix4f_t *matrix_ptr) noexcept {
	projection_transform = matrix_ptr;
}

void entity_renderer_t::render() noexcept {
	glUseProgram(scene->entity_shader->program_id);
	
	for (size_t i = 0; i < scene->entities.size(); i++) {
		scene->entity_shader->load_entity_transform(scene->entities[i].transform);

		glBindVertexArray(scene->entities[i].mesh->VAO_id);
		glEnableVertexAttribArray(0);
		// TODO: Consider switching to SHORT, but it'll require a different vector. You could template the mesh class.
		glDrawElements(GL_TRIANGLES, scene->entities[i].mesh->indices.size() * 3, GL_UNSIGNED_INT, nullptr);
		// NOTE: Not disabling for now because it's pointless.
	}
}
