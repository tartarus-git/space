#include <GLFW/glfw3.h>

#include <new>

#include "geometry/mesh_t.h"
#include "texturing/texture_t.h"
#include "entities/entity_t.h"
#include "scene/scene_t.h"
#include "rendering/camera_t.h"
#include "math/matrix4f_t.h"
#include "rendering/entity_renderer_t.h"

#include "debug/logger.h"
#include "exit_program.h"

void key_event(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		debug::logger << "[INFO]: escape key pressed, window should close flag set\n";
	}
}

mesh_t test_entity_mesh;
texture_t test_entity_texture;

// NOTE: As an example (assuming entity_shader had a non-default constructor and assuming we wouldn't be using a ptr):
// we can't non-default construct this shader directly in the global line, because the OpenGL context hasn't been setup
// by the time that would-be constructor would run. This is super important to keep in mind and it seems like something
// that's not hard to fall prey to when your in the flow of OOP programming.
// IDEA: Some sort of constructor flag (maybe [[]] type thing) that tells the compiler that you can't use the constructor in global scope.
// This'll get compile-time checked and save the user some headaches. Seems like a really good idea.
// TODO: Talk to std-discussion about it.
entity_shader_t *entity_shader_ptr;

scene_t scene;

camera_t camera;
matrix4f_t projection_matrix;

entity_renderer_t entity_renderer;

void game_init(GLFWwindow *window) noexcept {
	test_entity_mesh = mesh_t(
		{ { -1, -1, 0 }, { 1, -1, 0 }, { 0, 1, 0 } },
		{ { 0, 1, 2 } }
	);

	test_entity_texture = { };

	entity_t test_entity {
		&test_entity_mesh,
		&test_entity_texture,
		matrix4f_t::gen_identity()
	};

	// TODO: Wrap this in a smart pointer or something so you can manage the lifetime neatly.
	entity_shader_ptr = new (std::nothrow) entity_shader_t;
	if (!entity_shader_ptr) {
		debug::logger << "[ERROR]: failed to allocate entity shader on host\n";
		exit_program(EXIT_FAILURE);
	}

	scene = {
		{ test_entity },
		entity_shader_ptr
	};

	camera = {
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};

	int width;
	int height;
	glfwGetFramebufferSize(window, &width, &height);

	projection_matrix = matrix4f_t::gen_projection(0.1f, 1000, height / width, 1.5f);

	entity_renderer = entity_renderer_t(&scene, camera, &projection_matrix);
}

void game_loop(GLFWwindow *window) noexcept {
	entity_renderer.render();
}
