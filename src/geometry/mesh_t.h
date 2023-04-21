#pragma once

#define GL_GLEXT_PROTOTYPES	// NOTE: To avoid conflicts with other headers.
#include <GL/gl.h>

#include <vector>

#include "math/vector3f_t.h"
#include "math/vector3ui_t.h"

class mesh_t {
	void initialize() noexcept;

public:
	bool disposed = true;

	GLuint VAO_id;
	GLuint vertex_buffer_id;
	GLuint index_buffer_id;

	std::vector<vector3f_t> vertices;
	std::vector<vector3ui_t> indices;

	mesh_t() noexcept = default;
	mesh_t(std::vector<vector3f_t> vertices, 
	       std::vector<vector3ui_t> indices) noexcept;

	void update_device_buffers() const noexcept;

	mesh_t(const mesh_t& other) = delete;
	mesh_t& operator=(const mesh_t& other) = delete;

	mesh_t& operator=(mesh_t&& other) noexcept;
	mesh_t(mesh_t&& other) noexcept;

	void dispose() noexcept;

	~mesh_t() noexcept;

	static mesh_t gen_cube(vector3f_t position, vector3f_t scale) noexcept;
};
