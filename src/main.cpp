#define GL_GLEXT_PROTOTYPES	// NOTE: Defined even though we might not need it here to avoid conflicts with other headers.
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "main_game_code.h"

#include "debug/logger.h"
#include "exit_program.h"

void glfw_error_callback(int error, const char *description) noexcept {
	debug::logger << "glfw_error_callback: err_num: " << error << " description: " << description << '\n';
	exit_program(EXIT_FAILURE);
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

	GLFWwindow *window = glfwCreateWindow(640, 480, "space", nullptr, nullptr);
	if (!window) {
		debug::logger << "glfwCreateWindow() failed\n";
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	int frame_buffer_width, frame_buffer_height;
	glfwGetFramebufferSize(window, &frame_buffer_width, &frame_buffer_height);
	glViewport(0, 0, frame_buffer_width, frame_buffer_height);

	glfwSwapInterval(1);

	debug::logger << "[INFO]: calling game_init()\n";
	// NOTE: Function in main_game_code.h. I didn't call it init because that seems like a name that could lead to nasty collisions.
	game_init(window);
	debug::logger << "[INFO]: game_init() finished\n";

	glfwSetKeyCallback(window, glfw_key_callback);

	debug::logger << "[INFO]: window loop entered\n";

	while (!glfwWindowShouldClose(window)) {		// TODO: This doesn't close properly.
		game_loop(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	debug::logger << "[INFO]: window loop exited\n";

	glfwDestroyWindow(window);

	glfwTerminate();
}
