#define GL_GLEXT_PROTOTYPES	// NOTE: Defined even though we might not need it here to avoid conflicts with other headers.
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "main_game_code.h"

#include "debug/logger.h"
#include "exit_program.h"

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
	// NOTE: This function is found in main_game_code.h
	key_event(window, key, scancode, action, mods);
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
	glDepthFunc(GL_LESS);

	glClearColor(0, 0, 1, 0);

	debug::logger << "[INFO]: calling game_init()\n";
	// NOTE: Function in main_game_code.h. I didn't call it init because that seems like a name that could lead to nasty collisions.
	game_init(window);
	debug::logger << "[INFO]: game_init() finished\n";

	glfwSetKeyCallback(window, glfw_key_callback);

	debug::logger << "[INFO]: window loop entered\n";

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game_loop(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	debug::logger << "[INFO]: window loop exited\n";

	glfwDestroyWindow(window);

	glfwTerminate();
}
