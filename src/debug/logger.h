#pragma once

namespace debug {

	inline class logger_t {
	public:
		bool error = false;

		logger_t& operator<<(const char *input) noexcept;
		logger_t& operator<<(char input) noexcept;
	} logger;

}
