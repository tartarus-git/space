#define GL_GLEXT_PROTOTYPES	// NOTE: Defined even though we might not need it here to avoid conflicts with other headers.
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <new>

#include "game_t.h"

#include "debug/logger.h"
#include "exit_program.h"

// NOTE: We're forced to use a pointer and then heap allocate in order to be able to default construct this at exactly the right point.
// Suboptimal, but necessary. I wish C++ would be better about this.
// TODO: Mention this problem somewhere and see what can be done about it, maybe we can influence the standard somehow.
game_t *game;

void glfw_error_callback(int error, const char *description) noexcept {
	debug::logger << "[ERROR]: glfw_error_callback: err_num: " << error << " description: " << description << '\n';
	exit_program(EXIT_FAILURE);
}

void APIENTRY opengl_debug_message_callback(GLenum source, 
					    GLenum type, 
					    GLuint id, 
					    GLenum severity, 
					    GLsizei length, 
					    const GLchar *message, 
					    const void *user_param) noexcept {
	
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
	case GL_DEBUG_TYPE_PORTABILITY:
	case GL_DEBUG_TYPE_PERFORMANCE:
		debug::logger << "[ERROR]: ";
		break;
	default: debug::logger << "[INFO]: ";
	}

	debug::logger << "opengl_debug_message_callback: " << "type=" << type << " severity=" << severity << " message:\n" << message << '\n';
	// TODO: Put an fflush or something in exit_program so that we don't have to deal with flushing the debug output.

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
	case GL_DEBUG_TYPE_PORTABILITY:
	case GL_DEBUG_TYPE_PERFORMANCE:
		// NOTE: We continue even in the case of an error so that my own error handling code also has a chance to run and give
		// me more data. And so that compilation errors in the shaders all have a chance to get outputted, since there can
		// be multiple.
	default: break;
	}
}

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept {
	game->key_event(key, scancode, action, mods);
}

void glfw_cursor_position_callback(GLFWwindow *window, double x, double y) noexcept {
	game->cursor_position_event(x, y);
}

int main() {
	if (!glfwInit()) {
		debug::logger << "glfwInit() failed\n";
		return EXIT_FAILURE;
	}

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// NOTE: This is just a hint, so it may be ignored, more so than the above two hints, see docs.
	// Anyway, we use this because the context that we then create turns into a debug context with this,
	// which forces OpenGL to do debug output to our callback, which wasn't a guarantee before.
	// Obviously, this hint doesn't have to be followed, but it's an extra bit of security.
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow *window = glfwCreateWindow(640, 480, "space", nullptr, nullptr);
	if (!window) {
		debug::logger << "glfwCreateWindow() failed\n";
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// NOTE: In debug contexts, debug output starts off enabled.
	// We have this here in case the above hint for debug context was ignored.
	// Setting this again is no problem at all.
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(opengl_debug_message_callback, nullptr);

	int frame_buffer_width, frame_buffer_height;
	glfwGetFramebufferSize(window, &frame_buffer_width, &frame_buffer_height);
	glViewport(0, 0, frame_buffer_width, frame_buffer_height);

	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);		// NOTE: This depth function is necessary for skybox to work properly.

	glClearColor(0, 0, 1, 0);

	debug::logger << "[INFO]: constructing game_t object...\n";
	game = new (std::nothrow) game_t(window);
	if (!game) {
		debug::logger << "[ERROR]: failed to allocate game_t object in RAM\n";
		exit_program(EXIT_FAILURE);
	}
	debug::logger << "[INFO]: game_t construction finished\n";

	glfwSetKeyCallback(window, glfw_key_callback);

	// NOTES: cursor disabled hides the cursor, centers it on the screen, recenters automatically, and provides you with
	// absolute virtual mouse coords, as in where the mouse would be if it was unbounded. This means
	// these absolute coords can be directly turned into the current player rotation.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// NOTES: Raw mouse motion is as close as we can get to the actual behavior of the mouse on the surface, which is good
	// for 3D cameras, since it bypasses DPI scaling, custom speeds in the OS settings, and that sort of thing.
	// If that's not possible, we're forced to use the adjusted mouse movements, which will also work fine in most cases,
	// they're just not optimal.
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, true);
		debug::logger << "[INFO]: raw mouse motion supported and used\n";
	} else {
		debug::logger << "[INFO]: raw mouse motion unsupported, falling back on normal mouse movement\n";
	}
	glfwSetCursorPosCallback(window, glfw_cursor_position_callback);

	debug::logger << "[INFO]: window loop entered\n";

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game->loop();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	debug::logger << "[INFO]: window loop exited\n";

	delete game;

	glfwDestroyWindow(window);

	glfwTerminate();
}
