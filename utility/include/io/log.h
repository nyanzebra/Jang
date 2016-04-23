#pragma once

#include "configurator.h"
#include "file.h"

namespace ftl {
namespace io {

#define FTL_LOG_FILE(file) "../logs/" file ".txt"

class log : public file<char> {
public:
	log() = default;
	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	log(const T& filename) : file(filename, mode::kAPPEND | mode::kUPDATE) {}
	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	log(T&& filename) : file(std::forward<T>(filename), mode::kAPPEND | mode::kUPDATE) {}
	log(const log&) = default;
	log(log&&) = default;
	~log() = default;

	log& operator=(const log&) = default;

	const bool open(const std::string& filename) { return file<char>::open(file<char>::mode::kAPPEND, filename); }

	template<typename It>
	void write(It begin, It end) { file<char>::write<It>(begin, end); } 
};

}
}
