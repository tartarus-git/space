#define GL_GLEXT_PROTOTYPES	// NOTE: This shouldn't be necessary in this file, but I'm doing it anyway because why not? Just in case.
#include <GL/gl.h>

using void_func_ptr = void(*)();

extern "C" {
	void_func_ptr glXGetProcAddressARB(const GLubyte *func_name);
}

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

class does_glXGetProcAddress_exist {
	using no  = char(&)[1];
	using yes = char(&)[2];

	template <auto trash_data>
	consteval yes filter(decltype(glXGetProcAddress(trash_data))*) const;
	// NOTE: The above works because the function signature is in type dependant context. If it weren't, the validity
	// would be immediately evaluated, which moves it outside of SFINAE's jurisdiction because it doesn't wait until substitution.
	// NOTE: This must wait until substitution, making SFINAE work. See very similar behavior in static_assert condition.

	template <auto trash_data>
	consteval no filter(...) const;

public:
	consteval operator bool() const { return sizeof(filter<nullptr>(nullptr)) == sizeof(yes); }
};

// NOTE: The following wrapper is necessary for the constexpr to work, because it only filters out errors in substitution.
template <typename T = const GLubyte*>
void_func_ptr glXGetProcAddress_substitution_wrapper(T func_name) noexcept {
	if constexpr (does_glXGetProcAddress_exist{}) { return glXGetProcAddress(func_name); }
	return nullptr;
}

// TODO: Checks if the func is in the header, but not if the symbol can be linked to. Is the latter even possible in C++?
// NOTE: No noexcept because the signature in the glext.h header is without noexcept and I'm too lazy to change that.
#define GEN_IMPL(func_name, ret_type, invocation_args, ...) ret_type APIENTRY func_name(__VA_ARGS__) { \
	static ret_type (APIENTRY *ptr)(__VA_ARGS__) = nullptr; \
	if (!ptr) { \
		ptr = (decltype(ptr))glXGetProcAddressARB((const GLubyte*)#func_name); \
		if (!ptr) { \
			ptr = (decltype(ptr))glXGetProcAddress_substitution_wrapper((const GLubyte*)#func_name); \
			if (ptr) { return ptr invocation_args; } /* TODO: COMPILER BUG!!! There was ## between ptr and invocation_args but the compiler couldn't handle that for some reason. This works fine. RESEARCH FURTHER AND REPORT IF NECESSARY!!! */ \
			debug::logger << "[ERROR]: function bind for " << #func_name << " failed\n"; \
			exit_program(EXIT_FAILURE); \
		} \
	} \
	return ptr invocation_args; /* This works even if ptr() doesn't return anything because of language. */ \
}

GEN_IMPL(glUseProgram, void, (program), GLuint program)
