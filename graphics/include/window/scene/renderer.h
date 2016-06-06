#pragma once

#include <stack>

#include "renderable.h"
#include "renderer/vertex.h"

namespace jang {
namespace graphics {

#define VERTEX_SIZE sizeof(vertex)

#define VERTEX_POSITION_INDEX 0
#define VERTEX_COORDINATE_INDEX 1
#define VERTEX_TEXTURE_INDEX 2
#define VERTEX_COLOR_INDEX 3
#define VERTEX_NORMAL_INDEX 4
#define VERTEX_SHININESS_INDEX 5
#define VERTEX_SPECULAR_INDEX 6

#define BUFFER_SIZE	 CONFIGURATION["renderer"]["buffer size"].get<size_type>()

class renderer_policy_base {
public:
	renderer_policy_base() = default;
	renderer_policy_base(const renderer_policy_base&) = delete;
	renderer_policy_base(renderer_policy_base&&) = delete;
	virtual ~renderer_policy_base() = default;

	renderer_policy_base& operator=(const renderer_policy_base&) = delete;

protected:
	virtual void _init() = 0;

	virtual void _start() = 0;

	virtual void _add(const renderable& renderable) = 0;

	virtual void _render() = 0;

	virtual void _finish() = 0;

	virtual void _destroy() = 0;

	virtual const bool 	_can_add() = 0;

	void push(const glm::mat4& value) { _transformation_stack.push(value); }

	void pop() { _transformation_stack.pop(); }

protected:
	std::stack<const glm::mat4> _transformation_stack;
	std::size_t _indices;
	vertex* _data_buffer;
	GLuint _vertex_array, _vertex_buffer, _index_buffer;
	bool _can_render = false;
};

class renderer_policy_2d : public renderer_policy_base {
public:
	renderer_policy_2d() = default;
	renderer_policy_2d(const renderer_policy_2d&) = delete;
	renderer_policy_2d(renderer_policy_2d&&) = delete;
	~renderer_policy_2d() = default;

	renderer_policy_2d& operator=(const renderer_policy_2d&) = delete;

private:
	void _init();

	void _start();

	void _add(const renderable& renderable);

	void _render();

	void _finish();

	void _destroy();
};

class renderer_policy_3d : public renderer_policy_base {
public:
	renderer_policy_3d() = default;
	renderer_policy_3d(const renderer_policy_3d&) = delete;
	renderer_policy_3d(renderer_policy_3d&&) = delete;
	~renderer_policy_3d() = default;

	renderer_policy_3d& operator=(const renderer_policy_3d&) = delete;

private:
	void _init();

	void _start();

	void _add(const renderable& renderable);

	void _render();

	void _finish();

	void _destroy();
};

template <typename RenderPolicy, typename = std::enable_if_type<std::is_base_of<renderer_policy_base, RenderPolicy>::value>>
class renderer {
public:		 
	renderer() { RenderPolicy::_init(); }
	renderer(const renderer&) = delete;
	renderer(renderer&&) = delete;
	~renderer() { RenderPolicy::_destroy(); }

	renderer& operator=(const renderer&) = delete;

	void push(const glm::mat4& value) { RenderPolicy::push(value); }

	void pop() { RenderPolicy::pop(); }

	void start() { RenderPolicy::_start(); }

	void add(const renderable& renderable) { RenderPolicy::_add(renderable); }

	void render() { RenderPolicy::_render(); }

	void finish() { RenderPolicy::_finish(); }
};

typedef renderer<renderer_policy_2d> renderer_2d;
typedef renderer<renderer_policy_3d> renderer_3d;

}
}

