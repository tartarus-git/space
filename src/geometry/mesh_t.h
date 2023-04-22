#pragma once

#define GL_GLEXT_PROTOTYPES	// NOTE: To avoid conflicts with other headers.
#include <GL/gl.h>

#include <vector>

#include "math/vector3f_t.h"
#include "math/vector3ui_t.h"

class mesh_t {
public:
	bool disposed = true;

	std::vector<vector3f_t> vertices;
	std::vector<vector3ui_t> indices;

	GLuint VAO_id;
	GLuint vertex_buffer_id;
	GLuint index_buffer_id;

	constexpr mesh_t() noexcept = default;
	mesh_t(std::vector<vector3f_t> vertices, std::vector<vector3ui_t> indices) noexcept;

	mesh_t& operator=(const mesh_t& other) = delete;
	mesh_t(const mesh_t& other) = delete;

	mesh_t& operator=(mesh_t&& other) noexcept;
	mesh_t(mesh_t&& other) noexcept;

	void update_device_buffers() const noexcept;

	void dispose() noexcept;

	~mesh_t() noexcept;

	static mesh_t gen_cube(vector3f_t position, vector3f_t scale) noexcept;
};
