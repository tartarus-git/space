#pragma once

#include "math/vector3f_t.h"
#include "math/matrix4f_t.h"

class camera_t {
public:
	vector3f_t position;
	vector3f_t rotation;

	matrix4f_t gen_view_transform() const noexcept;
};
