#include "../../../include/window/scene/renderer.h"

#include "../../../include/settings.h"

namespace jang {
namespace graphics {

void renderer_policy_3d::_init() {
	glGenVertexArrays(1, &_vertex_array);
	glBindVertexArray(_vertex_array);

	glGenBuffers(1, &_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(VERTEX_POSITION_INDEX);
	glEnableVertexAttribArray(VERTEX_COORDINATE_INDEX);
	glEnableVertexAttribArray(VERTEX_TEXTURE_INDEX);
	glEnableVertexAttribArray(VERTEX_COLOR_INDEX);
	glEnableVertexAttribArray(VERTEX_NORMAL_INDEX);

	glVertexAttribPointer(VERTEX_POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(vertex, vertex::position)));
	glVertexAttribPointer(VERTEX_COORDINATE_INDEX, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(vertex, vertex::coordinate)));
	glVertexAttribPointer(VERTEX_TEXTURE_INDEX, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(vertex, vertex::texture)));
	glVertexAttribPointer(VERTEX_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE, (const GLvoid*)(offsetof(vertex, vertex::color)));
	glVertexAttribPointer(VERTEX_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(vertex, vertex::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	_indices = 0;
}

void renderer_policy_3d::_start() {
	if (!_can_render) {
		glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);

		_data_buffer = (vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		_can_render = true;
	}
}

void renderer_policy_3d::_add(const renderable& object) {
	if (!_can_render) {
		jang::io::console::output_line(DEFAULT_COLOR_SCHEME, "ERROR:\taction(s) was/were not surrounded with start and finish");
	}
	for (size_t i = 0; i < object.mesh().vertices().size(); i++) {
		_data_buffer->position = glm::vec3(_transformation_stack.top() * glm::vec4(object.mesh().vertices()[i] + object.position(), 0));
		_data_buffer->normal = glm::vec3(_transformation_stack.top() * glm::vec4(object.mesh().normals()[i], 0));
		_data_buffer->coordinate = object.mesh().coordinates()[i];
		_data_buffer->texture = object.material().texture().id();
		_data_buffer++;
	}

}

void renderer_policy_3d::_render() {
	if (_can_render) {
		glBindVertexArray(_vertex_array);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);

		glDrawElements(GL_TRIANGLE_STRIP, _indices * 3, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		_indices = 0;
	}
}

void renderer_policy_3d::_finish() {
	if (_can_render) {
		if (glUnmapBuffer(GL_ARRAY_BUFFER) != GL_TRUE) {

		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) != GL_TRUE) {

		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		_can_render = false;
	}
}

}
}
