#define GL_GLEXT_PROTOTYPES	// NOTE: This shouldn't be necessary in this file, but I'm doing it anyway because why not? Just in case.
#include <GL/gl.h>

#include "debug/logger.h"
#include "exit_program.h"

// NOTE: GLFW has a function for loading OpenGL core and extention functions, but it comes with a caveat that makes it undesirable.
// NOTE: We're using glXGetProcAddressARB, which is guaranteed to be part of the linux gl.h header and the libGL static lib.
// NOTE: There's also glXGetProcAddress, which might be in the header and static lib, but might also not be.
// NOTE: We're going to check if they're both here at compile-time and then try both for every function name if that's the case,
// because the OpenGL docs say that it's unclear which is to be used.
// NOTE: ALSO, unlike the Windows equivalents, these functions for Linux are context-independant, meaning we can get the pointers to the
// functions without a bound context. This also means that we might get a pointer to a function that we then can't actually use because
// the context isn't the right version or something.
// NOTE: This isn't a problem though because I'll simply only use functions that I know my context version definitely supports.

template <typename T>
using trash_bool_t = bool;

class does_glXGetProcAddress_exist {
	using no  = char(&)[1];
	using yes = char(&)[2];

	template <trash_bool_t<decltype(glXGetProcAddress)> = true>
	// NOTE: The above seems weird, but SFINAE still works here, even though the key part isn't in a dependant context.
	// That doesn't matter, because the validity is still only checked when substituting, making it fall under SFINAE's jurisdiction.
	consteval yes filter(void*);

	consteval no filter(...);

public:
	consteval operator bool() const { return sizeof(filter(nullptr)) == sizeof(yes); }
};

// TODO: Checks if the func is in the header, but not if the symbol can be linked to. Is the latter even possible in C++?
#define GEN_IMPL(func_name, ret_type, invocation_args, ...) ret_type APIENTRY func_name(__VA_ARGS__) { \
	static ret_type (APIENTRY *ptr)(__VA_ARGS__) = nullptr; \
	if (!ptr) { \
		ptr = (decltype(ptr))glXGetProcAddressARB(#func_name); \
		if (!ptr) { \
			if constexpr (does_glXGetProcAddress_exist{}) { \
				ptr = (decltype(ptr))glXGetProcAddress(#func_name); \
				if (ptr) { return ptr ## invocation_args; } \
			} \
			debug::logger << "[ERROR]: function bind for " << #func_name << " failed\n"; \
			exit_program(EXIT_FAILURE); \
		} \
	} \
	return ptr ## invocation_args; /* This works even if ptr() doesn't return anything because of language. */\
}

GEN_IMPL(glUseProgram, void, (program), GLuint program)
