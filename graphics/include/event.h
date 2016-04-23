#pragma once

#include <functional>
#include <future>

#include <dependencies.h>

namespace ftl {
namespace graphics {

//template<typename T, typename... Args>
//class event {
//public:
//	typedef std::function<T(Args&&...)> callback;
//	typedef std::function<bool(int, int, int, int)> activator;
//	typedef std::future<typename std::result_of<std::decay_t<T>(std::decay_t<Args>...)>> async;
//
//	event(const callback& callback, const activator& activator, GLFWwindow* window) : _callback(callback), _activator(activator), _window(window) {  }
//	event(callback&& callback, activator&& activator, GLFWwindow* window) : _callback(std::move(callback)), _activator(std::move(activator)), _window(window) { }
//	event(const event&) = default;
//	event(event&&) = default;
//	~event() = default;
//
//	void bind(const Args...& args);
//	void bind(Args...&& args);
//
//	T&& result();
//
//private:
//	void _setup();
//private:
//	T&& _result;
//	async _async;
//	callback _callback;
//	activator _activator;
//	GLFWwindow* _window;
//};

} // graphics
} // ftl
