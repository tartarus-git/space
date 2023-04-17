#include <GL/gl.h>

#include "shader_t.h"

#include <new>

#include "math/matrix4f_t.h"

#include "debug/logger.h"
#include "exit_program.h"

void shader_t::set_shader_source_and_compile(GLuint shader_id, const char *source_debug_id, const char *source) noexcept {
	glShaderSource(shader_id, 1, &source, nullptr);
	// TODO: What about errors on all these opengl functions. Figure that out later.
	glCompileShader(shader_id);

	GLint result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		debug::logger << "[ERROR]: shader compilation failed in set_shader_source_and_compile() (source debug ID: " << source_debug_id << "):\n";

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
		if (result == 0) {
			debug::logger << "[ERROR]: no info log available for compilation failure in set_shader_source_and_compile()\n";
			exit_program(EXIT_FAILURE);
		}

		char *info_log_buffer = new (std::nothrow) char[result];
		if (!info_log_buffer) {
			debug::logger << "[ERROR]: couldn't allocate info_log_buffer on host in set_shader_source_and_compile()\n";
			exit_program(EXIT_FAILURE);
		}

		glGetShaderInfoLog(shader, result, nullptr, info_log_buffer);
		debug::logger << info_log_buffer << '\n';

		delete[] info_log_buffer;

		exit_program(EXIT_FAILURE);
	}
}

shader_t::shader_t(const char *vertex_source_debug_id, const char *vertex_source, const char *fragment_source_debug_id, const char *fragment_source) noexcept {
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	if (vertex_shader_id == 0 || fragment_shader_id == 0) {
		debug::logger << "[ERROR]: glCreateShader() failed in shader_t constructor\n";
		exit_program(EXIT_FAILURE);
	}

	set_shader_source_and_compile(vertex_shader_id, vertex_source_debug_id, vertex_source);
	set_shader_source_and_compile(fragment_shader_id, fragment_source_debug_id, fragment_source);
	// TODO: Implement above function.

	GLuint program_id = glCreateProgram();
	if (program_id == 0) {
		debug::logger << "[ERROR]: glCreateProgram() failed in shader_t constructor\n";
		exit_program(EXIT_FAILURE);
	}

	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);

	glLinkProgram(program_id);

	GLint result;
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		debug::logger << "[ERROR]: shader program linkage failed in shader_t constructor (shader debug IDs: " << vertex_source_debug_id << " :: " << fragment_source_debug_id << "):\n";

		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &result);
		if (result == 0) {
			debug::logger << "[ERROR]: no info log available for linkage failure in shader_t constructor\n";
			exit_program(EXIT_FAILURE);
		}

		char *info_log_buffer = new (std::nothrow) char[result];
		if (!info_log_buffer) {
			debug::logger << "[ERROR]: couldn't allocate info_log_buffer on host in shader_t constructor\n";
			exit_program(EXIT_FAILURE);
		}

		glGetProgramInfoLog(program_id, result, nullptr, info_log_buffer);
		debug::logger << info_log_buffer << '\n';

		delete[] info_log_buffer;

		exit_program(EXIT_FAILURE);
	}

	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}

void shader_t::load_matrix4f(GLuint uniform_id, const matrix4f_t& matrix) noexcept {
	GLfloat buffer[16];
	std::memcpy(buffer, &matrix, sizeof(buffer));
	glUniformMatrix4fv(uniform_id, 1, GL_TRUE, buffer);
}
