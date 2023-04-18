#pragma once

#include <GLFW/glfw3.h>

void key_event(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept;
void cursor_position_event(GLFWwindow *window, double x, double y) noexcept;

void game_init(GLFWwindow *window) noexcept;

void game_loop(GLFWwindow *window) noexcept;
