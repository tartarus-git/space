#pragma once

#include "scene/scene_t.h"

class renderer_t {
public:
	const scene_t *scene;

	renderer_t() noexcept = default;
	renderer_t(const scene_t *scene) noexcept;

	renderer_t& operator=(const renderer_t& other) = delete;
	renderer_t(const renderer_t& other) = delete;
};
