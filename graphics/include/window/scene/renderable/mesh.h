#pragma once

#include <vector>
#include <dependencies.h>

namespace ftl {
namespace graphics {

class mesh {
public:
	mesh() = default;
	mesh(const mesh&) = default;
	mesh(mesh&&) = default;
	~mesh() = default;

	mesh& operator=(const mesh&) = default;

	std::vector<glm::vec3>& normals() { return _normals; }

	const std::vector<glm::vec3>& normals() const { return _normals; }

	template<template<typename> typename T, typename U, typename = std::enable_if_t<std::is_convertible<U, glm::vec3>::value && std::is_convertible<T<glm::vec3>, std::vector<glm::vec3>>::value>>
	void normals(const T<U>& value) { _normals = value; }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, glm::vec3>::value>>
	void normals(const std::vector<T>& value) { _normals = value; }


	std::vector<glm::vec3>& vertices() { return _vertices; }

	const std::vector<glm::vec3>& vertices() const { return _vertices; }

	template<template<typename>typename T, typename U, typename = std::enable_if_t<std::is_convertible<U, glm::vec3>::value && std::is_convertible<T<glm::vec3>, std::vector<glm::vec3>>::value>>
	void vertices(const T<U>& value) { _vertices = value; }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, glm::vec3>::value>>
	void vertices(const std::vector<T>& value) { _vertices = value; }


	std::vector<glm::vec2>& coordinates() { return _coordinates; }

	const std::vector<glm::vec2>& coordinates() const { return _coordinates; }

	template<template<typename>typename T, typename U, typename = std::enable_if_t<std::is_convertible<U, glm::vec3>::value && std::is_convertible<T<glm::vec2>, std::vector<glm::vec2>>::value>>
	void coordinates(const T<U>& value) { _coordinates = value; }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, glm::vec2>::value>>
	void coordinates(const std::vector<T>& value) { _coordinates = value; }


	std::vector<std::size_t>& indices() { return _indices; }

	const std::vector<std::size_t>& indices() const { return _indices; }

	template<template<typename>typename T, typename U, typename = std::enable_if_t<std::is_convertible<U, glm::vec3>::value && std::is_convertible<T<std::size_t>, std::vector<std::size_t>>::value>>
	void indices(const T<U>& value) { _indices = value; }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::size_t>::value>>
	void indices(const std::vector<T>& value) { _indices = value; }

private:
	std::vector<glm::vec3> _normals, _vertices;
	std::vector<glm::vec2> _coordinates;
	std::vector<std::size_t> _indices;
};

}
}
