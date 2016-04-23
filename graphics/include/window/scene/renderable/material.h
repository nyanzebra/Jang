#pragma once

#include "material/texture.h"

namespace ftl {
namespace graphics {

class material {
public:
	material() = default;
	material(const material&) = default;
	material(material&&) = default;
	~material() = default;

	material& operator=(const material&) = default;

	const graphics::texture& texture() const { return _texture; }
	void texture(const graphics::texture& texture) { _texture = texture; }
	void texture(graphics::texture&& texture) { _texture = std::move(texture); }

private:
	graphics::texture _texture;
};

}
}
