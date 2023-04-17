#pragma once

#include <vector>

#include "entities/entity_t.h"
#include "shaders/entity_shader_t.h"

struct scene_t {
	std::vector<entity_t> entities;
	const entity_shader_t *entity_shader;
};
