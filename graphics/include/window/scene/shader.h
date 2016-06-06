#pragma once

#include <dependencies.h>
#include <string>
#include <utility.h>

namespace jang {
namespace graphics {

class shader {
public:
	shader() = default;
	template <typename T, typename = std::enable_if_type<std::is_convertible<T, jang::string>::value>>
	shader(const T& vertex, const T& fragment) { compile(vertex, fragment); }
	template <typename T, typename = std::enable_if_type<std::is_convertible<T, jang::string>::value>>
	shader(T&& vertex, T&& fragment) { compile(std::forward<jang::string>(vertex), std::forward<jang::string>(fragment)); }
	~shader() { glDeleteProgram(_program); }

	const GLuint& value() { return _program; }


	template <typename T, typename = std::enable_if_type<std::is_convertible<T, jang::string>::value>>
	void compile(const T& vertex, const T& fragment);

	template <typename T, typename = std::enable_if_type<std::is_convertible<T, jang::string>::value>>
	void compile(T&& vertex, T&& fragment);

	void enable();

	const bool is_enabled() const;

	void disable();

	void uniform_floatx1(const char* uniform_object_name, const float& value);
	void uniform_floatx2(const char* uniform_object_name, const glm::vec2& value);
	void uniform_floatx3(const char* uniform_object_name, const glm::vec3& value);
	void uniform_floatx4(const char* uniform_object_name, const glm::vec4& value);

	void uniform_intx1(const char* uniform_object_name, const int& value);
	void uniform_intx2(const char* uniform_object_name, const glm::tvec2<int>& value);
	void uniform_intx3(const char* uniform_object_name, const glm::tvec3<int>& value);
	void uniform_intx4(const char* uniform_object_name, const glm::tvec4<int>& value);

	void uniform_uintx1(const char* uniform_object_name, const unsigned int& value);
	void uniform_uintx2(const char* uniform_object_name, const glm::tvec2<unsigned int>& value);
	void uniform_uintx3(const char* uniform_object_name, const glm::tvec3<unsigned int>& value);
	void uniform_uintx4(const char* uniform_object_name, const glm::tvec4<unsigned int>& value);

	void uniform_matrix4(const char* uniform_object_name, const glm::mat4& value);

	void uniform_float_vectorx1(const char* uniform_object_name, const unsigned int& count, const float* value);
	void uniform_float_vectorx2(const char* uniform_object_name, const unsigned int& count, const float* value);
	void uniform_float_vectorx3(const char* uniform_object_name, const unsigned int& count, const float* value);
	void uniform_float_vectorx4(const char* uniform_object_name, const unsigned int& count, const float* value);

	void uniform_int_vectorx1(const char* uniform_object_name, const unsigned int& count, const int* value);
	void uniform_int_vectorx2(const char* uniform_object_name, const unsigned int& count, const int* value);
	void uniform_int_vectorx3(const char* uniform_object_name, const unsigned int& count, const int* value);
	void uniform_int_vectorx4(const char* uniform_object_name, const unsigned int& count, const int* value);

	void uniform_uint_vectorx1(const char* uniform_object_name, const unsigned int& count, const unsigned int* value);
	void uniform_uint_vectorx2(const char* uniform_object_name, const unsigned int& count, const unsigned int* value);
	void uniform_uint_vectorx3(const char* uniform_object_name, const unsigned int& count, const unsigned int* value);
	void uniform_uint_vectorx4(const char* uniform_object_name, const unsigned int& count, const unsigned int* value);

private:
	bool _is_valid(const GLuint& program) const;

	void _shader_info(const GLuint& shader) const;

	void _program_info(const GLuint& program, const GLint& size) const;

	const GLuint _compile_vertex(const char* vertex) const;

	const GLuint _compile_fragment(const char* fragment) const;

private:
	bool _is_enabled;

	GLuint _program;
};

}
}
