#pragma once

#include <exception>

#include "../io/log.h"

namespace ftl {

#define FTL_EXCEPTION_MESSAGE(message) "ERROR:\nFILE: " __FILE__ "\nFUNCTION: " __FUNCTION__ "\nMESSAGE: " message

class exception : public std::exception {
public:
	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	exception(const T file, const T value) : std::exception(value) { _log.open(file); }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	exception(const T& file, const T& value) : std::exception(value) { _log.open(file); }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	exception(T&& file, T&& value) : std::exception(std::forward<T>(value)) { _log.open(std::forward<T>(file)); }

	exception(const exception& ex) : _log(ex._log) {}

	exception(exception&& ex) : _log(std::move(ex._log)) {}

	virtual ~exception() throw() = default;

	exception& operator=(const exception&) = default;

	virtual const char* what() throw() {
		std::string message = std::string(std::exception::what());
		_log.write(message.begin(), message.end());
		return message.c_str();
	}

protected:
	io::log _log;
};

}
