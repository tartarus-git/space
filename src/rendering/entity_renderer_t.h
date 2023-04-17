#pragma once

#include "renderer_t.h"

#include "scene/scene_t.h"
#include "camera_t.h"

#include "math/matrix4f_t.h"

// TODO: Add public keyword to all other inheritances as well.
class entity_renderer_t : public renderer_t {
public:
	matrix4f_t view_transform;
	const matrix4f_t *projection_transform;

	entity_renderer_t() noexcept = default;
	entity_renderer_t(const scene_t *scene, 
			  const camera_t& camera, 
			  const matrix4f_t *projection_matrix) noexcept;

	void overwrite_view_transform_with_camera(const camera_t& camera) noexcept;
	void apply_matrix_to_view_transform(const matrix4f_t& matrix) noexcept;

	void update_projection_transform(const matrix4f_t *matrix_ptr) noexcept;

	void render() noexcept;
};
