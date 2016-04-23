#include "../../../include/window/scene/shader.h"

namespace ftl {
namespace graphics {

void shader::enable() {
	if (!_is_enabled) {
		glUseProgram(_program);
		_is_enabled = true;
	}
}

void shader::disable() {
	if (_is_enabled) {
		glUseProgram(0);
		_is_enabled = false;
	}
}

void shader::_shader_info(const GLuint& shader) const {
	GLint compile_status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	char* error_message = new char[length];
	glGetShaderInfoLog(shader, length, &length, &error_message[0]);
	if (compile_status == GL_FALSE) {
		glDeleteShader(shader);
		throw ftl::exception(FTL_LOG_FILE("shader"), FTL_EXCEPTION_MESSAGE("shader could not compile..."));
	}
}

const GLuint shader::_compile_vertex(const char* vertex) const {
	GLuint shader_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader_vertex, 1, &vertex, NULL);
	glCompileShader(shader_vertex);

	_shader_info(shader_vertex);

	return shader_vertex;
}

const GLuint shader::_compile_fragment(const char* fragment) const {
	GLuint shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_fragment, 1, &fragment, NULL);
	glCompileShader(shader_fragment);

	_shader_info(shader_fragment);

	return shader_fragment;
}

template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
void shader::compile(const T& vertex, const T& fragment) {

	GLuint program = glCreateProgram();

	GLuint shader_vertex = _compile_vertex(vertex.c_str());
	GLuint shader_fragment = _compile_fragment(fragment.c_str());

	glAttachShader(program, shader_fragment);
	glAttachShader(program, shader_vertex);

	glLinkProgram(program);
	if (!_is_valid(program)) {
		throw ftl::exception(FTL_LOG_FILE("shader"), FTL_EXCEPTION_MESSAGE("shader program is invalid"));
	}

	glDeleteShader(shader_vertex);
	glDeleteShader(shader_fragment);

	_program = program;
}

void shader::uniform_floatx1(const char* name, const float& value) {
	glUniform1f(glGetUniformLocation(_program, name), value);
}
void shader::uniform_floatx2(const char* name, const glm::vec2& value) {
	glUniform2f(glGetUniformLocation(_program, name), value.x, value.y);
}
void shader::uniform_floatx3(const char* name, const glm::vec3& value) {
	glUniform3f(glGetUniformLocation(_program, name), value.x, value.y, value.z);
}
void shader::uniform_floatx4(const char* name, const glm::vec4& value) {
	glUniform4f(glGetUniformLocation(_program, name), value.x, value.y, value.z, value.w);
}

void shader::uniform_intx1(const char* name, const int& value) {
	glUniform1i(glGetUniformLocation(_program, name), value);
}
void shader::uniform_intx2(const char* name, const glm::tvec2<int>& value) {
	glUniform2i(glGetUniformLocation(_program, name), value.x, value.y);
}
void shader::uniform_intx3(const char* name, const glm::tvec3<int>& value) {
	glUniform3i(glGetUniformLocation(_program, name), value.x, value.y, value.z);
}
void shader::uniform_intx4(const char* name, const glm::tvec4<int>& value) {
	glUniform4i(glGetUniformLocation(_program, name), value.x, value.y, value.z, value.w);
}

void shader::uniform_uintx1(const char* name, const unsigned int& value) {
	glUniform1ui(glGetUniformLocation(_program, name), value);
}
void shader::uniform_uintx2(const char* name, const glm::tvec2<unsigned int>& value) {
	glUniform2ui(glGetUniformLocation(_program, name), value.x, value.y);
}
void shader::uniform_uintx3(const char* name, const glm::tvec3<unsigned int>& value) {
	glUniform3ui(glGetUniformLocation(_program, name), value.x, value.y, value.z);
}
void shader::uniform_uintx4(const char* name, const glm::tvec4<unsigned int>& value) {
	glUniform4ui(glGetUniformLocation(_program, name), value.x, value.y, value.z, value.w);
}

void shader::uniform_matrix4(const char* name, const glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(_program, name), 1, GL_FALSE, &value[0][0]);
}

void shader::uniform_float_vectorx1(const char* name, const unsigned int& count, const float* value) {
	glUniform1fv(glGetUniformLocation(_program, name), count, value);
}
void shader::uniform_float_vectorx2(const char* name, const unsigned int& count, const float* value) {
	glUniform2fv(glGetUniformLocation(_program, name), count, value);
}
void shader::uniform_float_vectorx3(const char* name, const unsigned int& count, const float* value) {
	glUniform3fv(glGetUniformLocation(_program, name), count, value);
}
void shader::uniform_float_vectorx4(const char* name, const unsigned int& count, const float* value) {
	glUniform4fv(glGetUniformLocation(_program, name), count, value);
}

void shader::uniform_int_vectorx1(const char* name, const unsigned int& count, const int* value) {
	glUniform1iv(glGetUniformLocation(_program, name), count, value);
}
void shader::uniform_int_vectorx2(const char* name, const unsigned int& count, const int* value) {
	glUniform2iv(glGetUniformLocation(_program, name), count, value);
}
void shader::uniform_int_vectorx3(const char* name, const unsigned int& count, const int* value) {
	glUniform3iv(glGetUniformLocation(_program, name), count, value);
}
void shader::uniform_int_vectorx4(const char* name, const unsigned int& count, const int* value) {
	glUniform4iv(glGetUniformLocation(_program, name), count, value);
}

void shader::uniform_uint_vectorx1(const char* name, const unsigned int& count, const unsigned int* value) {
	glUniform1uiv(glGetUniformLocation(_program, name), count, value);

}
void shader::uniform_uint_vectorx2(const char* name, const unsigned int& count, const unsigned int* value) {
	glUniform2uiv(glGetUniformLocation(_program, name), count, value);

}
void shader::uniform_uint_vectorx3(const char* name, const unsigned int& count, const unsigned int* value) {
	glUniform3uiv(glGetUniformLocation(_program, name), count, value);

}
void shader::uniform_uint_vectorx4(const char* name, const unsigned int& count, const unsigned int* value) {
	glUniform4uiv(glGetUniformLocation(_program, name), count, value);

}

}
}
