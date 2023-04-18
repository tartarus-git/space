#include "mesh_t.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <vector>

#include "math/vector3f_t.h"
#include "math/vector3ui_t.h"

void mesh_t::initialize() noexcept {
	// TODO: Check about error handling here.
	glGenVertexArrays(1, &VAO_id);
	glBindVertexArray(VAO_id);

	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &index_buffer_id);
}

mesh_t::mesh_t() noexcept { initialize(); }

mesh_t::mesh_t(std::vector<vector3f_t> vertices, 
	       std::vector<vector3ui_t> indices) noexcept : 
		vertices(vertices), 
		indices(indices) {

	initialize();
	update_device_buffers();

}

void mesh_t::update_device_buffers() const noexcept {
	glBindVertexArray(VAO_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, 
		     vertices.size() * sizeof(vector3f_t), 
		     vertices.data(), 
		     GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		     indices.size() * sizeof(vector3ui_t), 
		     indices.data(),
		     GL_STATIC_DRAW);
}
