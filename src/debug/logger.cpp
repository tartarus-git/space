#include "logger.h"

#include <cstdio>

namespace debug {

	logger_t& logger_t::operator<<(const char *input) noexcept {
		if (printf(input) < 0) { error = true; }
		return *this;
	}

	logger_t& logger_t::operator<<(char input) noexcept {
		if (printf("%c", input) < 0) { error = true; }
		return *this;
	}

};
