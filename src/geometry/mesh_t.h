#pragma once

#define GL_GLEXT_PROTOTYPES	// NOTE: To avoid conflicts with other headers.
#include <GL/gl.h>

#include <vector>

#include "math/vector3f_t.h"
#include "math/vector3ui_t.h"

class mesh_t {
	void initialize() noexcept;

public:
	GLuint VAO_id;
	GLuint vertex_buffer_id;
	GLuint index_buffer_id;

	std::vector<vector3f_t> vertices;
	std::vector<vector3ui_t> indices;

	mesh_t() noexcept;

	mesh_t(std::vector<vector3f_t> vertices, 
	       std::vector<vector3ui_t> indices) noexcept;

	void update_device_buffers() const noexcept;

	// TODO: destructor
};
