#define GL_GLEXT_PROTOTYPES	// NOTE: Same reason as other instances of this.
#include <GL/gl.h>

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

	logger_t& logger_t::operator<<(GLuint input) noexcept {
		if (printf("%u", input) < 0) { error = true; }
		return *this;
	}

	logger_t& logger_t::operator<<(int input) noexcept {
		if (printf("%d", input) < 0) { error = true; }
		return *this;
	}

};
