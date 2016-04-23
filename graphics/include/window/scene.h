#pragma once

#include <dependencies.h>

#include <map>
#include <memory>

#include "scene/camera.h"
#include "scene/renderer.h"
#include "scene/shader.h"

namespace ftl {
namespace graphics {

#define RENDERER_MODE_3D 57
#define RENDERER_MODE_2D 26

class scene {
public:
	enum class mode : char {
		kNONE,
		k3D,
		k2D,
		kCUSTOM3D,
		kCUSTOM2D
	};		  

	scene() = default;
	scene(const scene&) = delete;
	scene(scene&&) = default;
	~scene() = default;

	scene& operator=(const scene&) = delete;

	void bind_shader(const shader& shader);

	void unbind_shader();

	template<typename RendererPolicy>
	void bind_renderer();

	void unbind_renderer();

	void add(const std::size_t& key, renderable value);

	void remove(const std::size_t& key);

	void render();			 

private:
	bool _is_shader_bound = false, _is_renderer_bound = false;
	mode _render_mode = mode::kNONE;
	glm::mat4  _projection_matrix = glm::mat4(1);
	glm::mat4 _transformation = glm::mat4(1);
	shader _shader;
	std::map< std::size_t, renderable> _renderable_map;
	camera _camera;
	renderer_2d _renderer_2d;
	renderer_3d _renderer_3d;
	//allow for custom 2d and 3d
};

} // graphics
} // ftl
