#pragma once

void key_event(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept;

void init(GLFWwindow *window) noexcept;

void game_loop(GLFWwindow *window) noexcept;
