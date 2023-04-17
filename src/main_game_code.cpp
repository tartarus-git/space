void key_event(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		debug::logger << "[INFO]: escape key pressed, winodw should close flag set\n";
	}
}

renderer_t renderer;

void init(GLFWwindow *window) noexcept {
	renderer = renderer_t(scene);
}

void game_loop(GLFWwindow *window) noexcept {
	renderer.render();
}
