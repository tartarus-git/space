#pragma once

class vector3f_t {
public:
	float x;
	float y;
	float z;

	constexpr vector3f_t() noexcept = default;
	constexpr vector3f_t(float x, float y, float z) noexcept : x(x), y(y), z(z) { }

	constexpr vector3f_t operator-() const noexcept {
		return { -x, -y, -z };
	}

	constexpr vector3f_t operator+(const vector3f_t& other) const noexcept {
		return { x + other.x, y + other.y, z + other.z };
	}

	constexpr vector3f_t& operator+=(const vector3f_t& other) noexcept {
		*this = *this + other;
		return *this;
	}

	constexpr vector3f_t operator-(const vector3f_t& other) const noexcept {
		return { x - other.x, y - other.y, z - other.z };
	}
};
