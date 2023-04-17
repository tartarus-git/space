#pragma once

#include "vector3f_t.h"

class vector4f_t {
public:
	float x;
	float y;
	float z;
	float w;

	// NOTE: List initialization doesn't work if there are any constructors in the class, but we want list initialization.
	// Luckily, list initialization defaults to normal constructor-based construction, and it can work great if the proper constructor
	// is made available, like I've done below.
	// NOTE: std::initializer_list is something else: that compiler magic construct is used when boundless list initialization
	// is required (think array list initialization without explicit size parameter) on classes.
	constexpr vector4f_t(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
	constexpr vector4f_t(const vector3f_t& vec3f) noexcept : x(vec3f.x), y(vec3f.y), z(vec3f.z), w(1) { }

	constexpr vector3f_t xyz() const noexcept { return vector3f_t(x, y, z); }
};
