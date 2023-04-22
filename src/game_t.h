#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "geometry/mesh_t.h"
#include "texturing/texture_t.h"
#include "entities/entity_t.h"
#include "shaders/entity_shader_t.h"
#include "texturing/skybox_texture_t.h"
#include "skybox/skybox_t.h"
#include "shaders/skybox_shader_t.h"
#include "scene/scene_t.h"
#include "rendering/camera_t.h"
#include "math/vector2i_t.h"
#include "math/vector3f_t.h"
#include "math/vector4f_t.h"
#include "math/matrix4f_t.h"
#include "rendering/entity_renderer_t.h"
#include "rendering/skybox_renderer_t.h"

#include "baked_assets/skybox_face_images.h"

#include "debug/logger.h"

#define PLAYER_MOVE_SPEED 0.01f
#define PLAYER_LOOK_SPEED 0.001f

// NOTE: Uncomment to use mouse for rotation instead of arrow keys. Doesn't work in WSL because of interop difficulties.
//#define MOUSE_ROTATION

// NOTE: All of this is inside the game_t class because that way, we can initialize all the variables as member variables and have everything
// be alright. Before, we were doing them as globals, but we couldn't construct them the way we wanted because the OpenGL context hadn't
// been created yet. We had to default construct them and then set them from the init function, which is annoying and ugly.
// More importantly though, it caused some of our classes to have to deal with things like moving and copying even though
// they normally wouldn't need to care about that stuff (see shader classes, which don't deal with that, but they would have to if these
// were all globals, either that, or be pointers that we new in the init function, but then they don't automatically destruct,
// which is annoying).
// Additionally, the destructors were getting called after the OpenGL context had been disposed, which probably doesn't matter,
// but it's just not clean. In order to destruct them before that, we would have to manually call dispose() functions, which is annoying.
// By main.cpp managing the lifetime of this class, we can use all of the comfort features and nice stuff that we want at no cost or annoyance.

// TODO: This global constructor even though context doesn't exist problem is a trap that's not hard to fall into when you're in the flow of
// OOP programming. There exists a potential for the language to help you out here. There could be an attribute for constructors
// that tells the compiler to prevent the constructor from being called from global scope. This compile-time check could save
// the programmer some head-aches.
// TODO: Talk to std_discussion about it.

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

class game_t {
	static vector2i_t get_current_frame_size(GLFWwindow *window) noexcept {
		int width;
		int height;
		glfwGetFramebufferSize(window, &width, &height);
		return { width, height };
	}

public:
	// NOTE: Can't use parenthesis even though I want to because the language gets confused because it looks like a function.
	// Language assumes function, which is why we need to use curly braces to prevent that from happening.
	mesh_t test_entity_mesh {
		{ { -1, -1, -1 }, { 1, -1, -1 }, { 0, 1, -1 } },
		{ { 0, 1, 2 } }
	};

	entity_t test_entity {
		&test_entity_mesh,
		matrix4f_t::gen_identity()
	};

	entity_shader_t entity_shader;

	mesh_t skybox_mesh = mesh_t::gen_cube({ 0, 0, 0 }, { 1, 1, 1 });

	const unsigned char *face_images[6] {
		baked_assets::skybox_face_image_0,
		baked_assets::skybox_face_image_1,
		baked_assets::skybox_face_image_2,
		baked_assets::skybox_face_image_3,
		baked_assets::skybox_face_image_4,
		baked_assets::skybox_face_image_5
	};
	const size_t face_image_sizes[6] {
		baked_assets::skybox_face_image_size_0,
		baked_assets::skybox_face_image_size_1,
		baked_assets::skybox_face_image_size_2,
		baked_assets::skybox_face_image_size_3,
		baked_assets::skybox_face_image_size_4,
		baked_assets::skybox_face_image_size_5
	};
	skybox_texture_t skybox_texture { face_images, face_image_sizes };

	skybox_t skybox {
		&skybox_mesh,
		&skybox_texture
	};

	skybox_shader_t skybox_shader;

	scene_t scene {
		{ test_entity },
		&entity_shader,

		skybox,
		&skybox_shader
	};

	camera_t camera {
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};

	GLFWwindow *window;

	vector2i_t frame_size;

	matrix4f_t projection_matrix;

	entity_renderer_t entity_renderer;
	skybox_renderer_t skybox_renderer;

	game_t(GLFWwindow *window) noexcept : window(window), 
					      frame_size(get_current_frame_size(window)), 
					      projection_matrix(matrix4f_t::gen_projection(0.1f, 1000, frame_size.x / frame_size.y, 1)), 
					      entity_renderer(&scene, camera, &projection_matrix), 
					      skybox_renderer(&scene, camera, &projection_matrix) { }

	void key_event(int key, int scancode, int action, int mods) noexcept {
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

	void cursor_position_event(double x, double y) noexcept {
		virtual_cursor_x = x;
		virtual_cursor_y = y;
		cursor_position_changed = true;
	}

	matrix4f_t player_controlled_camera_rotation_matrix = matrix4f_t::gen_identity();
	matrix4f_t player_controlled_camera_inverse_rotation_matrix = matrix4f_t::gen_identity();

	void loop() noexcept {
		entity_renderer.render();
		skybox_renderer.render();
		// TODO: Add other renderer.
		// Also add that glEnable depth stuff in the skybox renderer class so that it works nicely. It's in the comments of thinmatrix
		// video.

#ifdef MOUSE_ROTATION
		if (cursor_position_changed) {
			// NOTE: Don't worry about thread-safety, I'm pretty sure the callbacks are on this thread as well.
			// NOTE: Yes, we could have the callback set camera directly, but that's less safe because
			// other code could use camera and have it changed unexpectedly, whereas the cursor variables can change all they want,
			// because we only check them here.
			camera.rotation.x = virtual_cursor_x * PLAYER_LOOK_SPEED;
			camera.rotation.y = virtual_cursor_y * PLAYER_LOOK_SPEED;
			player_controlled_camera_rotation_matrix = matrix4f_t::gen_fps_rotation(camera.rotation);
			player_controlled_camera_inverse_rotation_matrix = matrix4f_t::gen_fps_rotation(-camera.rotation);
			entity_renderer.view_transform = player_controlled_camera_inverse_rotation_matrix * matrix4f_t::gen_translate(-camera.position);
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
			player_controlled_camera_rotation_matrix = matrix4f_t::gen_fps_rotation(camera.rotation);
			player_controlled_camera_inverse_rotation_matrix = matrix4f_t::gen_fps_rotation(-camera.rotation);
			entity_renderer.view_transform = player_controlled_camera_inverse_rotation_matrix * matrix4f_t::gen_translate(-camera.position);
			skybox_renderer.view_transform = player_controlled_camera_inverse_rotation_matrix;
		}
#endif

		bool position_changed = false;
		vector4f_t position_change_vector { 0, 0, 0, 1 };
		if (key_flags::w) {
			position_change_vector.z = -PLAYER_MOVE_SPEED;
			position_changed = true;
		}
		if (key_flags::a) {
			position_change_vector.x = -PLAYER_MOVE_SPEED;
			position_changed = true;
		}
		if (key_flags::s) {
			position_change_vector.z = PLAYER_MOVE_SPEED;
			position_changed = true;
		}
		if (key_flags::d) {
			position_change_vector.x = PLAYER_MOVE_SPEED;
			position_changed = true;
		}
		if (position_changed) {
			//camera.position = -((entity_renderer.view_transform * vector4f_t(0, 0, 0, 1)).xyz());
			// NOTE: The above doesn't work because negating the end vector doesn't mean doing the steps in reverse.
			// That only holds true for translation alone (maybe scaling alone too I suppose, and maybe translation and scaling
			// together). For translation + rotation, that just yields garbage. We have to do it in a more complicated way.
			// We could probably invert the matrix, but that's computationally expensive.

			vector4f_t proposed_change = player_controlled_camera_rotation_matrix * position_change_vector;
			proposed_change.y = 0;		// NOTE: Filter out vertical movement for FPS behavior.

			camera.position += proposed_change.xyz();

			vector3f_t inversely_rotated_proposed_change = (player_controlled_camera_inverse_rotation_matrix * proposed_change).xyz();
			entity_renderer.view_transform = matrix4f_t::gen_translate(-inversely_rotated_proposed_change) * entity_renderer.view_transform;
		}
	}
};
