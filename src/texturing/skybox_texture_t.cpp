#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "skybox_texture_t.h"

#include <utility>

#include "stb/stb_image.h"

#include "debug/logger.h"
#include "exit_program.h"

// NOTE: SUPER DUPER UNDEFINED BEHAVIOR IF YOU CALL THIS BEFORE THE MAIN CONSTRUCTOR (the non-default constructor)
void skybox_texture_t::generate_cpu_copies() noexcept {
	if (face_image_data[0]) { return; }		// NOTE: We use the first element as a freed flag.

	for (size_t i = 0; i < 6; i++) {
		int channels_in_image_source;
		// NOTE: cast isn't really necessary in this case but it's clean, so I'm keeping it.
		face_image_data[i] = stbi_load_from_memory((const stbi_uc*)face_image_ptrs[i], 
							   face_image_sizes[i], 
							   &face_image_widths[i], 
							   &face_image_heights[i], 
							   &channels_in_image_source, 
							   3);
		if (!face_image_data[i]) {
			debug::logger << "[ERROR]: stbi_load_from_memory() failed\n";
			exit_program(EXIT_FAILURE);
		}
	}
}

skybox_texture_t::skybox_texture_t(const unsigned char *face_images[6], const size_t face_image_sizes[6]) noexcept {

	for (size_t i = 0; i < 6; i++) {
		this->face_image_ptrs[i] = face_images[i];
		this->face_image_sizes[i] = face_image_sizes[i];
	}

	generate_cpu_copies();

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for (size_t i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
			     0, 
			     GL_RGB, 
			     face_image_widths[i], 
			     face_image_heights[i], 
			     0, 
			     GL_RGB, 
			     GL_UNSIGNED_BYTE, 
			     face_image_data[i]);
	}

	// TODO: We only do the mipmap stuff so that high-def skyboxes don't do moire-patterns when we render them.
	// This is fine, but we only need one LOD, which is the one that the screen size works best with, since we never move
	// relative to the skybox. Only when the screen size changes do we need to regenerate that one LOD that we need.
	// So, stop generating all the mipmaps and only generate the one that you actually need. You can use the GPU and do kernel
	// passes to make the averaging fast.
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// trilinear filtering, AFAIK

	disposed = false;

}

skybox_texture_t& skybox_texture_t::operator=(skybox_texture_t&& other) noexcept {
	dispose();

	disposed = other.disposed;

	for (size_t i = 0; i < 6; i++) {
		face_image_ptrs[i] = other.face_image_ptrs[i];
		face_image_sizes[i] = other.face_image_sizes[i];

		face_image_data[i] = other.face_image_data[i];
		face_image_widths[i] = other.face_image_widths[i];
		face_image_heights[i] = other.face_image_heights[i];
	}

	id = other.id;

	other.disposed = true;

	return *this;
}

skybox_texture_t::skybox_texture_t(skybox_texture_t&& other) noexcept { *this = std::move(other); }

// NOTE: SUPER DUPER UNDEFINED BEHAVIOR IF YOU CALL THIS BEFORE THE MAIN CONSTRUCTOR (the non-default constructor)
void skybox_texture_t::delete_cpu_copies() noexcept {
	if (!face_image_data[0]) { return; }		// NOTE: We use the first element as a freed flag.

	for (size_t i = 0; i < 6; i++) { stbi_image_free(face_image_data[i]); }
	face_image_data[0] = nullptr;
}

void skybox_texture_t::dispose() noexcept {
	if (disposed) { return; }
	delete_cpu_copies();
	glDeleteTextures(1, &id);
	disposed = true;
}

skybox_texture_t::~skybox_texture_t() noexcept { dispose(); }
