#pragma once

#include <vector>

struct scene_t {
	std::vector<entity_t> entities;
	const entity_shader_t *entity_shader;
};
