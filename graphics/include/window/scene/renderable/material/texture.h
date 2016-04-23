#pragma once

#include <string>

#include <dependencies.h>

namespace ftl {
namespace graphics {

class texture {
public:
	texture() = default;
	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	texture(T file_name, bool generate_mipmap = false) { load(file_name, generate_mipmap); }
	texture(const texture&) = default;
	texture(texture&&) = default;
	~texture() = default;

	texture& operator=(const texture&) = default;

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	const bool load(T file_name, bool generate_mipmap = false);

	void bind(); 

	void unbind();

	const GLuint& id() const { return _id; }

private:
	bool _is_bound = false;

	GLuint _id = 0;

	std::string _file_name;
	GLvoid* _data;
	size_t _width;
	size_t _height;
	size_t _depth;
	size_t _bits_per_pixel;
};

}
}
