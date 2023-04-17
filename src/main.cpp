#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

#include "debug/logger.h"

void glfw_error_callback(int error, const char *description) noexcept {
	debug::logger << "glfw_error_callback: err_num: " << error << " description: " << description << '\n';
}

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		debug::logger << "[INFO]: escape key pressed, winodw should close flag set\n";
	}
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

	glfwSetKeyCallback(window, glfw_key_callback);

	debug::logger << "[INFO]: window loop entered\n";

	while (!glfwWindowShouldClose(window)) {		// TODO: This doesn't close properly.
		// TODO: everything
	}

	debug::logger << "[INFO]: window loop exited\n";

	glfwDestroyWindow(window);

	glfwTerminate();
}
