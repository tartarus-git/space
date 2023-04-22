#pragma once

class vector2i_t {
public:
	int x;
	int y;

	constexpr vector2i_t(int x, int y) noexcept : x(x), y(y) { }
};
