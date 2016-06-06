#include "../include/event.h"

namespace jang {
namespace graphics {

//template <typename T, typename... Args>
//void event<T, ...Args>::_setup() {
//	glfwSetKeyCallback(_window,
//		[&](GLFWwindow* window, int key, int scancode, int action, int mods) {
//		if (_activator(key, scancode, action, mods)) {
//			if (std::is_same<T, void>::value) {
//				_async.get();
//			} else {
//				_result = _async.get();
//			}
//		}
//	}
//	);
//}
//
//template <typename T, typename... Args>
//void event<T, ...Args>::bind(const Args...& args) {
//	_async = std::async(std::launch::async, _callback, args...);
//	_setup();
//}
//
//template <typename T, typename... Args>
//void event<T, ...Args>::bind(Args...&& args) {
//	_async = std::async(std::launch::async, _callback, std::forward<Args>(args)...);
//	_setup();
//}
//
//template <typename T, typename ...Args>
//T&& event<T, ...Args>::result() {
//	return std::move(_result);
//}

}
}