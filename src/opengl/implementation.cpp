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

// TODO: Member detector idiom for detecting that second function?
