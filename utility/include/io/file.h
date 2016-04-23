#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <vector>	

#include "console.h"
#include "../types.h"

namespace ftl {
namespace io {

template<typename T = char>
class file {
public:
	enum class mode : char {
		kREAD = 0x1,
		kWRITE = 0x2,
		kAPPEND = 0x3,
		kUPDATE = 0x4,
		kBINARY = 0x8
	};

	file() = default;
	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	file(const T& filename, const mode mode = mode::kREAD) : _filename(filename) { open(mode); }
	template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
	file(T&& filename, const mode mode = mode::kREAD) : _filename(std::move(filename)) { open(mode); }
	file(const file& file) : _filename(file._filename), _file(file._file), _is_open(file._is_open) {}
	file(file&& file) : _filename(std::move(file._filename)), _file(std::move(file._file)), _is_open(std::move(file._is_open)) {}
	virtual ~file() { close(); };

	file& operator=(const file&) = default;

	const bool open(const mode mode, const std::string& filename = "") { return _open(mode, filename); }

	const size_t size() { fseek(_file, 0, SEEK_END); return ftell(_file) / sizeof(T); }

	template<typename It>
	void write(It begin, It end) { _write(begin, end); }

	template<typename It>
	void read(It begin, It end, size_type offset = 0) { return _read(begin, end, offset); }

	void close() { _close(); }

private:
	void _force_close() {
		if (_file) {
			fflush(_file);
			fclose(_file);
		}
	}

	const bool _open(const mode mode, const std::string& filename = "") {
		std::lock_guard<std::mutex> lock(_file_lock);
		try {
			if (filename.compare("") != 0) {
				_filename = filename;
			}
			if (!_is_open) {
				std::string access;

				switch (mode) {
				case mode::kAPPEND:
					access = "a";
					break;
				case mode::kWRITE:
					access = "w";
					break;
				case mode::kREAD:
				default:
					access = "r";
					break;
				}
				if (((int)mode & (int)mode::kUPDATE) == 1) {
					access += "+";
				}

				if (((int)mode & (int)mode::kBINARY) == 1) {
					access += "b";
				}
				  
#if defined(_WIN32) || defined(_WIN64)
				errno_t err = fopen_s(&_file, _filename.c_str(), access.c_str());
				if (err) {
					return false;
				}
#else
				_file = fopen(_filename.c_str(), access.c_str());
				if (!_file) {
					return false;
				}
#endif				

				_is_open = true;
				return true;
			}
		} catch (std::runtime_error& err) {
			_force_close();
			console::output_error(RED, WHITE, err.what());
			return false;
		}

		return false;
	}

	template<typename It>
	void _write(It begin, It end) {
		std::lock_guard<std::mutex> lock(_file_lock);

		try {
			if (_is_open && begin < end) {
				fwrite(&(*begin), sizeof(typename std::iterator_traits<It>::value_type), std::distance(begin, end), _file);
			}
		} catch (std::runtime_error& err) {
			_force_close();
			console::output_error(RED, WHITE, err.what());
		}
	}

	template <typename It>
	void _read(It begin, It end, size_type offset) {
		std::lock_guard<std::mutex> lock(_file_lock);

		typedef typename std::iterator_traits<It>::value_type value_type;

		try {
			if (_is_open) {
				if (end < begin) {
					if (_file) {
						fseek(_file, 0, SEEK_END);
						end = begin + ftell(_file);
					}
				}
				if (_file) {
					fseek(_file, offset * sizeof(value_type), SEEK_SET);
					fread(&(*begin), sizeof(value_type), std::distance(begin, end), _file);
				}
			}
		} catch (std::runtime_error& err) {
			_force_close();
			console::output_line(RED, WHITE, err.what());
		}
	}

	void _close() {
		if (_is_open) {
			_force_close();
			_is_open = false;
		}
	}

protected:
	std::string _filename;

private:
	std::mutex _file_lock;
	bool _is_open = false;
	FILE* _file;
};

} // io
} // ftl
