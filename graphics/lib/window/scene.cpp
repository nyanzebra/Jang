#include "../../include/window/scene.h"

namespace ftl {
namespace graphics {

void scene::bind_shader(const shader& shader) {
	if (!_is_shader_bound) {
		_shader = shader;
		_shader.enable();
		_is_shader_bound = true;
	}
}

void scene::unbind_shader() {
	if (_is_shader_bound) {
		_shader.disable();
		_is_shader_bound = false;
	}
}

template<typename Policy>
void scene::bind_renderer() {
	_renderer = std::unique_ptr<renderer<Policy>>(new renderer<Policy>());
}

void scene::unbind_renderer() {}

void scene::add(const std::size_t & key, renderable value) {}

void scene::remove(const std::size_t & key) {}

void scene::render() {
	if (_is_renderer_bound) {

		switch (_render_mode) {
		case mode::kNONE:
			return;
		case mode::k2D:
			_renderer_2d.start();  
			
			for (auto&& renderable : _renderable_map) {	 
				_renderer_2d.push();
				_renderer_2d.add(renderable.second);
				_renderer_2d.pop();
			}

			_renderer_2d.finish();

			_renderer_2d.render();
			break;
		case mode::k3D:
			_renderer_3d.start();

			for (auto&& renderable : _renderable_map) {
				_renderer_3d.push();
				_renderer_3d.add(renderable.second);
				_renderer_3d.pop();
			}

			_renderer_3d.finish();

			_renderer_3d.render();
			break;
		case mode::kCUSTOM2D:
			break;
		case mode::kCUSTOM3D:
			break;
		default:
			return;
		}
	}
}

}
}
