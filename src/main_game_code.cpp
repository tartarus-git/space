#include <GLFW/glfw3.h>

#include "main_game_code.h"

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

#define PLAYER_MOVE_SPEED 0.001f
#define PLAYER_LOOK_SPEED 0.001f

// NOTE: Uncomment to use mouse for rotation instead of arrow keys. Doesn't work in WSL because of interop difficulties.
//#define MOUSE_ROTATION

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

namespace key_flags {
	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;

	bool up_arrow = false;
	bool left_arrow = false;
	bool down_arrow = false;
	bool right_arrow = false;
};

void key_event(GLFWwindow *window, int key, int scancode, int action, int mods) noexcept {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
			debug::logger << "[INFO]: escape key pressed, window should close flag set\n";
		}
		break;
	case GLFW_KEY_W:
		switch (action) {
		case GLFW_PRESS: key_flags::w = true; break;
		case GLFW_RELEASE: key_flags::w = false;
		}
		break;
	case GLFW_KEY_A:
		switch (action) {
		case GLFW_PRESS: key_flags::a = true; break;
		case GLFW_RELEASE: key_flags::a = false;
		}
		break;
	case GLFW_KEY_S:
		switch (action) {
		case GLFW_PRESS: key_flags::s = true; break;
		case GLFW_RELEASE: key_flags::s = false;
		}
		break;
	case GLFW_KEY_D:
		switch (action) {
		case GLFW_PRESS: key_flags::d = true; break;
		case GLFW_RELEASE: key_flags::d = false;
		}
		break;
	case GLFW_KEY_UP:
		switch (action) {
		case GLFW_PRESS: key_flags::up_arrow = true; break;
		case GLFW_RELEASE: key_flags::up_arrow = false;
		}
		break;
	case GLFW_KEY_LEFT:
		switch (action) {
		case GLFW_PRESS: key_flags::left_arrow = true; break;
		case GLFW_RELEASE: key_flags::left_arrow = false;
		}
		break;
	case GLFW_KEY_DOWN:
		switch (action) {
		case GLFW_PRESS: key_flags::down_arrow = true; break;
		case GLFW_RELEASE: key_flags::down_arrow = false;
		}
		break;
	case GLFW_KEY_RIGHT:
		switch (action) {
		case GLFW_PRESS: key_flags::right_arrow = true; break;
		case GLFW_RELEASE: key_flags::right_arrow = false;
		}
		break;
	}
}

double virtual_cursor_x = 0;
double virtual_cursor_y = 0;
bool cursor_position_changed = false;

void cursor_position_event(GLFWwindow *window, double x, double y) noexcept {
	virtual_cursor_x = x;
	virtual_cursor_y = y;
	cursor_position_changed = true;
}

void game_init(GLFWwindow *window) noexcept {
	test_entity_mesh = mesh_t(
		{ { -1, -1, -1 }, { 1, -1, -1 }, { 0, 1, -1 } },
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

	projection_matrix = matrix4f_t::gen_projection(0.1f, 1000, width / height, 1);

	entity_renderer = entity_renderer_t(&scene, camera, &projection_matrix);
}

void game_loop(GLFWwindow *window) noexcept {
	entity_renderer.render();

#ifdef MOUSE_ROTATION
	if (cursor_position_changed) {
		// NOTE: Don't worry about thread-safety, I'm pretty sure the callbacks are on this thread as well.
		// NOTE: Yes, we could have the callback set camera directly, but that's less safe because
		// other code could use camera and have it changed unexpectedly, whereas the cursor variables can change all they want,
		// because we only check them here.
		camera.rotation.x = virtual_cursor_x * PLAYER_LOOK_SPEED;
		camera.rotation.y = virtual_cursor_y * PLAYER_LOOK_SPEED;
		entity_renderer.view_transform = camera.gen_view_transform();
		cursor_position_changed = false;
	}
#else
	bool rotation_changed = false;
	if (key_flags::up_arrow) {
		camera.rotation.z += PLAYER_LOOK_SPEED;
		rotation_changed = true;
	}
	if (key_flags::left_arrow) {
		camera.rotation.y += PLAYER_LOOK_SPEED;
		rotation_changed = true;
	}
	if (key_flags::down_arrow) {
		camera.rotation.z -= PLAYER_LOOK_SPEED;
		rotation_changed = true;
	}
	if (key_flags::right_arrow) {
		camera.rotation.y -= PLAYER_LOOK_SPEED;
		rotation_changed = true;
	}
	if (rotation_changed) {
		entity_renderer.view_transform = camera.gen_view_transform();
	}
#endif

	if (key_flags::w) {
		camera.position.z -= PLAYER_MOVE_SPEED;
		// NOTE: Don't worry, gen_translate is constexpr, and so is the xyz constructor of vector3f_t.
		entity_renderer.view_transform = matrix4f_t::gen_translate({ 0, 0, PLAYER_MOVE_SPEED }) * entity_renderer.view_transform;
	}
	if (key_flags::a) {
		camera.position.x -= PLAYER_MOVE_SPEED;
		entity_renderer.view_transform = matrix4f_t::gen_translate({ PLAYER_MOVE_SPEED, 0, 0 }) * entity_renderer.view_transform;
	}
	if (key_flags::s) {
		camera.position.z += PLAYER_MOVE_SPEED;
		entity_renderer.view_transform = matrix4f_t::gen_translate({ 0, 0, -PLAYER_MOVE_SPEED }) * entity_renderer.view_transform;
	}
	if (key_flags::d) {
		camera.position.x += PLAYER_MOVE_SPEED;
		entity_renderer.view_transform = matrix4f_t::gen_translate({ -PLAYER_MOVE_SPEED, 0, 0 }) * entity_renderer.view_transform;
	}
}
