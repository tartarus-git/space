#include "camera_t.h"

#include "math/matrix4f_t.h"

matrix4f_t camera_t::gen_view_transform() const noexcept {
	matrix4f_t result = matrix4f_t::gen_fps_rotation(-rotation);
	result = result * matrix4f_t::gen_translate(-position);
	return result;
}
