#include "camera_t.h"

#include "math/matrix4f_t.h"

matrix4f_t camera_t::gen_view_transform() const noexcept {
	matrix4f_t result = matrix4f::gen_rotation(rotation);
	result = matrix4f::gen_translate(position) * result;
	return result;
	// TODO: Make sure the above is good.
}
