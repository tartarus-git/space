#pragma once

#include <vector>

#include "math/vector3f_t.h"
#include "math/vector3ui_t.h"

struct mesh_t {
	std::vector<vector3f_t> vertices;
	std::vector<vector3ui_t> indices;
};
