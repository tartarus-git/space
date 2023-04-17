#pragma once

class vector3f_t {
public:
	float x;
	float y;
	float z;

	constexpr vector3f_t operator+(const vector3f_t& other) const noexcept {
		return { x + other.x, y + other.y, z + other.z };
	}

	constexpr vector3f_t operator-(const vector3f_t& other) const noexcept {
		return { x - other.x, y - other.y, z - other.z };
	}
};
