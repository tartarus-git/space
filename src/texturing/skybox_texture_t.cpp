#include "skybox_texture_t.h"

#include "stb/stb_image.h"

#include "debug/logger.h"
#include "exit_program.h"

skybox_texture_t::skybox_texture_t(const char *face_images[6], 
				   size_t face_image_sizes[6]) noexcept {

	glGenTextures(1, &cube_map_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);

	for (size_t i = 0; i < 6; i++) {
		int channels_in_image_source;
		face_image_data[i] = stbi_load_from_memory((const stbi_uc*)face_images[i], 
							   face_image_sizes[i], 
							   &face_image_widths[i], 
							   &face_image_heights[i], 
							   &channels_in_image_source, 
							   3);
		if (!face_image_data[i]) {
			debug::logger << "[ERROR]: stbi_load_from_memory() failed\n";
			exit_program(EXIT_FAILURE);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
			     0, 
			     GL_RGB, 
			     face_image_widths[i], 
			     face_image_heights[i], 
			     0, 
			     GL_RGB, 
			     GL_UNSIGNED_BYTE, 
			     face_image_data[i]);

		// TODO: Do min and mag filter, but only if they work with mipmaps. Figure out all of that stuff for textures and cube maps.

		// TODO: See if below works with cube maps.
		//glGenerateMipmap(GL_TEXTURE_2D);	// TODO: Research about this and anisotropic filtering and all that.
	}

}

skybox_texture_t::~skybox_texture_t() noexcept {
	for (size_t i = 0; i < 6; i++) {
		// TODO: Consider making a free function for if you want to free CPU memory in order to save memory usage.
		stbi_image_free(face_image_data[i]);
	}
}
