#pragma once

#include <cmath>

#include "vector3f_t.h"
#include "vector4f_t.h"

class matrix4f_t {
public:
	float data[4][4];

	constexpr const float* operator[](size_t i) const noexcept { return data[i]; }
	constexpr float* operator[](size_t i) noexcept { return data[i]; }

	constexpr matrix4f_t operator*(const matrix4f_t& other) const noexcept {
		matrix4f_t result;
		for (size_t y = 0; y < 4; y++) {
			for (size_t x = 0; x < 4; x++) {
				result[y][x] = (*this)[y][0] * other[0][x] + 
					       (*this)[y][1] * other[1][x] + 
					       (*this)[y][2] * other[2][x] + 
					       (*this)[y][3] * other[3][x];
			}
		}
		return result;
	}

	constexpr vector4f_t operator*(const vector4f_t& other) const noexcept {
		return {
			other.x * (*this)[0][0] + 
			other.y * (*this)[0][1] + 
			other.z * (*this)[0][2] + 
			other.w * (*this)[0][3],

			other.x * (*this)[1][0] + 
			other.y * (*this)[1][1] + 
			other.z * (*this)[1][2] + 
			other.w * (*this)[1][3],

			other.x * (*this)[2][0] + 
			other.y * (*this)[2][1] + 
			other.z * (*this)[2][2] + 
			other.w * (*this)[2][3],

			other.x * (*this)[3][0] + 
			other.y * (*this)[3][1] + 
			other.z * (*this)[3][2] + 
			other.w * (*this)[3][3]
		};
	}

	static constexpr matrix4f_t gen_identity() noexcept {
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}

	static constexpr matrix4f_t gen_scale(vector3f_t factors) noexcept {
		return {
			factors.x,         0,         0, 0,
			        0, factors.y,         0, 0,
			        0,         0, factors.z, 0,
			        0,         0,         0, 1
		};
	}

	static constexpr matrix4f_t gen_translate(vector3f_t offsets) noexcept {
		return {
			1, 0, 0, offsets.x,
			0, 1, 0, offsets.y,
			0, 0, 1, offsets.z,
			0, 0, 0,         1
		};
	}

	// TODO: Make this constexpr after replacing those trig functions with constexpr versions
	static matrix4f_t gen_rotation(vector3f_t rotations) noexcept {
		float cosx = cosf(rotations.x);
		float sinx = sinf(rotations.x);
		float nsinx = -sinx;

		float cosy = cosf(rotations.y);
		float siny = sinf(rotations.y);
		float nsiny = -siny;

		float cosz = cosf(rotations.z);
		float sinz = sinf(rotations.z);
		float nsinz = -sinz;

		return matrix4f_t {
			cosx, nsinx,    0, 0,
			sinx,  cosx,    0, 0,
			   0,     0,    1, 0,
			   0,     0,    0, 1
		} * matrix4f_t {
			cosy,     0, siny, 0,
			   0,     1,    0, 0,
			nsiny, 0,    cosy, 0,
			0, 0, 0, 1
		} * matrix4f_t {
			1, 0, 0, 0,
			0, cosz, nsinz, 0,
			0, sinz, cosz, 0,
			0, 0, 0, 1
		};
	}

	static matrix4f_t gen_fps_rotation(vector3f_t rotations) noexcept {
		float cosx = cosf(rotations.x);
		float sinx = sinf(rotations.x);
		float nsinx = -sinx;

		float cosy = cosf(rotations.y);
		float siny = sinf(rotations.y);
		float nsiny = -siny;

		float cosz = cosf(rotations.z);
		float sinz = sinf(rotations.z);
		float nsinz = -sinz;

		// TODO: Understand this new ordering and also check to make sure the ordering for the other, space-wise one, also makes sense and that you understand it.
		return matrix4f_t {
			1, 0, 0, 0,
			0, cosz, nsinz, 0,
			0, sinz, cosz, 0,
			0, 0, 0, 1
		} * matrix4f_t {
			cosy,     0, siny, 0,
			   0,     1,    0, 0,
			nsiny, 0,    cosy, 0,
			0, 0, 0, 1
		} * matrix4f_t {
			cosx, nsinx,    0, 0,
			sinx,  cosx,    0, 0,
			   0,     0,    1, 0,
			   0,     0,    0, 1
		};
	}

	// NOTE: aspect_ratio is height/width ---> NO ITS NOT, its the opposite while we're using that pre-built mechanism.
	static matrix4f_t gen_projection(float near_plane, float far_plane, float aspect_ratio, float FOV) noexcept {
		return {
			1 / (aspect_ratio * tanf(FOV / 2)), 0, 0, 0,
			0, 1 / (tanf(FOV / 2)), 0, 0,
			0, 0, -(far_plane + near_plane) / (far_plane - near_plane), -2 * far_plane * near_plane / (far_plane - near_plane),
			0, 0, -1, 0
		};

		// NOTE: Turns out, projections matrices are a little bit more complicated in OpenGL because
		// of NDC and clip coordinate spaces and such. Those are basically an optimization AFAIK, but it requires
		// more work in this matrix. TODO: Understand the code we've pasted above.
		/*return {
			1,                0,          0,     0,
			0, 1 / aspect_ratio,          0,     0,
			0,                0,          1,     0,
			0,                0, -tanf(FOV / 2), 0
		};*/
	}
};
