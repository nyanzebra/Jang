#pragma once

#include <exception>
#include <map>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <string>

#include <dependencies.h>

#include "window/scene.h"
#include "settings.h"

namespace ftl {
namespace graphics {

class window {
public:
	window() { _init(); }
	window(const window&) = delete;
	window(window&&) = delete;
	~window() { glfwTerminate(); }

	window& operator=(const window) = delete;

	const bool create(const std::string& window_name);

	void setup();

	void clear();

	void update();

	const bool should_close();

	void render();

	void bind(std::unique_ptr<scene> scene);

	void destroy();

	void position(const int& x, const int& y);

private:
	const bool _init();

	void _diagnostic();

	void _frames_per_second();

private:
	GLFWwindow* _window;
	std::map<std::string, std::unique_ptr<window>> _windows;
	std::unique_ptr<scene> _scene;
	size_type _frames = 0;
	std::chrono::system_clock::time_point _last_time = std::chrono::system_clock::now();
};

class window_exception : public std::runtime_error {

};

} // graphics
} // ftl
