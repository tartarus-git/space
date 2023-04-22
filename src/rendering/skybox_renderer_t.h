#pragma once

// NOTE: We could put the corresponding shaders inside the renderers, and that would work great and be clean and nice, but there is
// one problem with that. I want to be able to create multiple renderers of the same type that see things with different perspectives
// and all that, but that's not practical with the proposed model because all the shader VRAM data would be stored multiple times.
// A solution to that problem with shared memory between the shader instances would be super clunky and weird.
// Instead, we opt to keep the current system and use a central shader from multiple renderers.

#include "renderer_t.h"

#include "scene/scene_t.h"
#include "camera_t.h"

#include "math/matrix4f_t.h"

class skybox_renderer_t : public renderer_t {
public:
	matrix4f_t view_transform;
	const matrix4f_t *projection_transform;

	skybox_renderer_t() noexcept = default;
	skybox_renderer_t(const scene_t *scene, 
			  const camera_t& camera, 
			  const matrix4f_t *projection_matrix) noexcept;

	void overwrite_view_transform_with_camera(const camera_t& camera) noexcept;
	void apply_matrix_to_view_transform(const matrix4f_t& matrix) noexcept;

	void update_projection_transform(const matrix4f_t *matrix_ptr) noexcept;

	void render() noexcept;
};
