#include "../include/window.h"
#include  "io/console.h"
#include "io/configurator.h"

namespace ftl {
namespace graphics {

void window::_diagnostic() {
	io::console::output_line(DEFAULT_COLOR_SCHEME, "Renderer: ", glGetString(GL_RENDERER));
	io::console::output_line(DEFAULT_COLOR_SCHEME, "OpenGL version supported: ", glGetString(GL_VERSION));
	io::console::output_line(DEFAULT_COLOR_SCHEME, "GLEW version supported: ", glewGetString(GLEW_VERSION));
	io::console::output_line(DEFAULT_COLOR_SCHEME, "GLFW version supported: ", glfwGetVersionString());
	io::console::output_line();
}

void window::_frames_per_second() {
	_frames++;
	if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - _last_time).count() > 1) {
		io::console::output_line(DEFAULT_COLOR_SCHEME, _frames, " fps");
		_frames = 0;
		_last_time = std::chrono::system_clock::now();
	}
}

const bool window::_init() {
	if (!glfwInit()) {
		io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR: could not start GLFW3");
		return false;
	}

#if __APPLE__
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	return true;
}

void window::position(const int& x, const int& y) {
	glfwSetWindowPos(_window, x, y);
}

const bool window::create(const std::string& window_name) {
	_window = glfwCreateWindow(DEFAULT_WINDOW_POSITION, window_name.c_str(), NULL, NULL);

	glfwSetWindowPos(_window, DEFAULT_WINDOW_POSITION);

	if (!_window) {
		ftl::io::console::output_error(RED, WHITE, "ERROR: could not open window with GLFW\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(_window);

	glfwSetWindowUserPointer(_window, this);

	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		ftl::io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:");
		return false;
	}

	_diagnostic();

	return true;
}

void window::setup() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_3D);
	glDepthFunc(GL_LESS);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glClearColor(0.5, 0.5, 0.5, 1);
}

void window::bind(std::unique_ptr<scene> scene) { _scene = std::move(scene); }

void window::render() {
	_scene->render();
	_frames_per_second();
}

void window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::update() {
	GLenum err = glGetError();
	if (err) {
		switch (err) {
		case GL_INVALID_ENUM:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tinvalid enum");
			break;
		case GL_INVALID_VALUE:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tinvalid value");
			break;
		case GL_INVALID_OPERATION:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tinvalid operation");
			break;
		default:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tunknown");
			break;
		}
	}
	glfwPollEvents();
	glfwSwapBuffers(_window);
}

const bool window::should_close() {
	return glfwWindowShouldClose(_window) == 1;
}

void window::destroy() {
	if (_scene) {
		_scene.release();
	}
	glfwDestroyWindow(_window);
}

}
}
