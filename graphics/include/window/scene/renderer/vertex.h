#pragma once

#include <dependencies.h>

namespace ftl {
namespace graphics {

struct vertex {
	vertex() = default;
	vertex(const vertex&) = default;
	vertex(vertex&&) = default;
	~vertex() = default;	   

	vertex& operator=(const vertex&) = default;

	friend bool operator<(vertex& lhs, const vertex& rhs) { return memcmp((void*)&lhs, (void*)&rhs, sizeof(vertex)) > 0; }

	friend bool operator>(vertex& lhs, const vertex& rhs) { return memcmp((void*)&lhs, (void*)&rhs, sizeof(vertex))< 0; }

	friend bool operator>=(vertex& lhs, const vertex& rhs) { return !(lhs < rhs); }

	friend bool operator<=(vertex& lhs, const vertex& rhs) { return !(rhs > lhs); }

	friend bool operator==(vertex& lhs, const vertex& rhs) { return memcmp((void*)&lhs, (void*)&rhs, sizeof(vertex)) == 0; }

	bool operator<(const vertex& rhs) const { return (*this < rhs); }

	bool operator>(const vertex& rhs) const { return (*this > rhs); }

	bool operator>=(const vertex& rhs) const { return (*this >= rhs); }

	bool operator<=(const vertex& rhs) const { return (*this <= rhs); }

	bool operator==(const vertex& rhs) const { return (*this == rhs); }		

	float shininess;
	glm::vec4 color;
	glm::vec3 position, normal, specular;
	glm::vec2 coordinate;
	size_t texture;
};

}
}
