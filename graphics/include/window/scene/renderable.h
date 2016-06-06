#pragma once

#include <dependencies.h>

#include "renderable/material.h"
#include "renderable/mesh.h"

namespace jang {
namespace graphics {

#define ORIGIN_POINT glm::vec3(0)

class renderable {
public:
	typedef graphics::material material_type;
	typedef graphics::mesh mesh_type;
	typedef std::size_t size_type;
	typedef glm::vec4 color_type;
	typedef std::size_t compressed_color_type;
	typedef glm::vec3 position_type;

	renderable() = default;
	renderable(const renderable&) = default;
	renderable(renderable&&) = default;
	~renderable() = default;

	renderable& operator=(const renderable&) = default;

	const compressed_color_type& color() const { return _color; }
	void color(size_type r, size_type g, size_type b, size_type a) { _color = (a << 24) | (b << 16) | (g << 8) | (r); }
	void color(const color_type& value) { color((value.w * 255.0f), (value.z * 255.0f), (value.y * 255.0f), (value.x * 255.0f)); }
	void color(color_type&& value) { color((std::move(value.w) * 255.0f), (std::move(value.z) * 255.0f), (std::move(value.y) * 255.0f), (std::move(value.x) * 255.0f)); }

	const position_type& position() const { return _position; }
	void position(const position_type& value) { _position = value; }
	void position(position_type&& value) { _position = std::move(value); }

	void rotate(const glm::quat& quaternion, const glm::vec3& point = glm::vec3(1)) {
		//_position = ORIGIN_POINT + (glm::quat::toMat4(quaternion) * (point - ORIGIN_POINT));
	}

	const glm::vec3& size() const { return _size; }
	void size(const glm::vec3& value) { _size = value; }
	void size(glm::vec3&& value) { _size = std::move(value); }

	const material_type& material() const { return _material; }
	void material(const material_type& value) { _material = value; }
	void material(material_type&& value) { _material = std::move(value); }

	const mesh_type& mesh() const { return _mesh; }
	void mesh(const mesh_type& value) { _mesh = value; }
	void mesh(mesh_type&& value) { _mesh = std::move(value); }

private:
	size_type _color;
	position_type _position;
	glm::vec3 _size;
	material_type _material;
	mesh_type _mesh;
};

}
}
