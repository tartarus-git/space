#include <GLFW/glfw3.h>

#include "rendering/entity_renderer_t.h"

#include "debug/logger.h"

void key_event(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		debug::logger << "[INFO]: escape key pressed, winodw should close flag set\n";
	}
}

entity_renderer_t entity_renderer;

void init(GLFWwindow *window) noexcept {
}

void game_loop(GLFWwindow *window) noexcept {
	entity_renderer.render();
}
