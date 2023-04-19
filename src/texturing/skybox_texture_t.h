#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "stb/stb_image.h"

class skybox_texture_t {
public:
	stbi_uc *face_image_data[6];
	size_t face_image_widths[6];
	size_t face_image_heights[6];

	GLuint cube_map_texture;

	skybox_texture_t(const char *face_images[6], size_t face_image_sizes[6]) noexcept;

	~skybox_texture_t() noexcept;
};
