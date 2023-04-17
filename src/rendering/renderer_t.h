#pragma once

#include "scene/scene_t.h"

class renderer_t {
public:
	const scene_t *scene;

	renderer_t() noexcept = default;
	renderer_t(const scene_t *scene) noexcept;
};
