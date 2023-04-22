#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "stb/stb_image.h"

class skybox_texture_t {
public:
	bool disposed = true;

	const unsigned char *face_image_ptrs[6];
	size_t face_image_sizes[6];

	stbi_uc *face_image_data[6];
	int face_image_widths[6];
	int face_image_heights[6];

	GLuint id;

	void generate_cpu_copies() noexcept;

	constexpr skybox_texture_t() noexcept = default;
	skybox_texture_t(const unsigned char *face_images[6], const size_t face_image_sizes[6]) noexcept;

	skybox_texture_t& operator=(const skybox_texture_t& other) = delete;
	skybox_texture_t(const skybox_texture_t& other) = delete;

	skybox_texture_t& operator=(skybox_texture_t&& other) noexcept;
	skybox_texture_t(skybox_texture_t&& other) noexcept;

	void delete_cpu_copies() noexcept;

	void dispose() noexcept;

	~skybox_texture_t() noexcept;
};
