#pragma once

#define GL_GLEXT_PROTOTYPES	// NOTE: Same reason as other instances of this.
#include <GL/gl.h>

namespace debug {

	inline class logger_t {
	public:
		bool error = false;

		logger_t& operator<<(const char *input) noexcept;
		logger_t& operator<<(char input) noexcept;

		logger_t& operator<<(GLuint input) noexcept;
		logger_t& operator<<(int input) noexcept;
	} logger;

}
