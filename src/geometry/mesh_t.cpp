#include "mesh_t.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <vector>
#include <utility>

#include "math/vector3f_t.h"
#include "math/vector3ui_t.h"

mesh_t::mesh_t(std::vector<vector3f_t> vertices, 
	       std::vector<vector3ui_t> indices) noexcept : 
		vertices(vertices), 
		indices(indices) {

	// TODO: Check about error handling here.
	glGenVertexArrays(1, &VAO_id);
	glBindVertexArray(VAO_id);

	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &index_buffer_id);

	update_device_buffers();

	disposed = false;

}

mesh_t& mesh_t::operator=(mesh_t&& other) noexcept {
	dispose();

	disposed = other.disposed;

	vertices = std::move(other.vertices);
	indices = std::move(other.indices);

	VAO_id = other.VAO_id;
	vertex_buffer_id = other.vertex_buffer_id;
	index_buffer_id = other.index_buffer_id;

	other.disposed = true;

	return *this;
}

mesh_t::mesh_t(mesh_t&& other) noexcept { *this = std::move(other); }

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

void mesh_t::dispose() noexcept {
	if (disposed) { return; }

	glDeleteBuffers(1, &index_buffer_id);
	glDeleteBuffers(1, &vertex_buffer_id);
	glDeleteVertexArrays(1, &VAO_id);

	indices.clear();
	indices.shrink_to_fit();
	vertices.clear();
	vertices.shrink_to_fit();

	disposed = true;
}

mesh_t::~mesh_t() noexcept { dispose(); }

mesh_t mesh_t::gen_cube(vector3f_t position, vector3f_t scale) noexcept {
	return mesh_t(
		{
			vector3f_t{  1,  1,  1 } * scale + position,
			vector3f_t{  1,  1, -1 } * scale + position,
			vector3f_t{  1, -1,  1 } * scale + position,
			vector3f_t{  1, -1, -1 } * scale + position,
			vector3f_t{ -1,  1,  1 } * scale + position,
			vector3f_t{ -1,  1, -1 } * scale + position,
			vector3f_t{ -1, -1,  1 } * scale + position,
			vector3f_t{ -1, -1, -1 } * scale + position,
		},
		{
			{ 0, 1, 2 },
			{ 1, 2, 3 },
			{ 4, 5, 6 },
			{ 5, 6, 7 },
			{ 0, 4, 1 },
			{ 5, 4, 1 },
			{ 1, 5, 3 },
			{ 7, 5, 3 },
			{ 2, 6, 3 },
			{ 7, 6, 3 },
			{ 2, 6, 3 },
			{ 0, 2, 6 },
			{ 6, 4, 0 },
		}
	);
}
