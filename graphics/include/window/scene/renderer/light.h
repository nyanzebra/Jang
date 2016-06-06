#pragma once

#include <dependencies.h>

namespace jang {
namespace graphics {

struct vertex {
	vertex() = default;
	vertex(const vertex&) = default;
	vertex(vertex&&) = default;
	~vertex() = default;

	vertex& operator=(const vertex&) = default;

	friend bool operator<(vertex& lhs, const vertex& rhs) { return memcmp((void*)&lhs, (void*)&rhs, sizeof(vertex)) > 0; }

	friend bool operator<(vertex& lhs, const vertex& rhs) { return memcmp((void*)&lhs, (void*)&rhs, sizeof(vertex)) > 0; }

	friend bool operator>=(vertex& lhs, const vertex& rhs) { return !(lhs < rhs); }

	friend bool operator<=(vertex& lhs, const vertex& rhs) { return !(rhs > lhs); }

	friend bool operator==(vertex& lhs, const vertex& rhs) { return memcmp((void*)&lhs, (void*)&rhs, sizeof(vertex)) == 0; }

	bool operator<(const vertex& rhs) const { return (*this < rhs); }

	bool operator>(const vertex& rhs) const { return (*this > rhs); }

	bool operator>=(const vertex& rhs) const { return (*this >= rhs); }

	bool operator<=(const vertex& rhs) const { return (*this <= rhs); }

	bool operator==(const vertex& rhs) const { return (*this == rhs); }

	glm::vec3 position, intensity;
	float attenuation, ambient_coefficient;
};

}
}
