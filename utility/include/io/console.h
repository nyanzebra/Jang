#pragma once

#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <direct.h>

#define BLACK 0
#define GREY 8
#define BLUE 9
#define GREEN 10
#define CYAN 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15

#define DARKER(color) color - 8
#define AS_BACKGROUND(color) color << 4
#define AS_ASCII(color) ((color < 10) ? color + 48: color + 55) 

#ifndef CWD
#define CWD _getcwd
#endif

#else

#include <unistd.h>

#define RESET 0
#define BOLD 1
#define UNDERSCORE 4
#define BLINK 5
#define CONCEAL 8

#define BLACK 31
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define PURPLE 35
#define CYAN 36
#define WHITE 37

#define AS_BACKGROUND(color) color + 10

#ifndef CWD
#define CWD getcwd
#endif

#endif

namespace ftl {
namespace io {

class console {
public:
	static void print_working_directory() { _output(std::cout, BLACK, WHITE, _current_working_directory()); }

	template <typename T, typename... U>
	static void output(const size_t& foreground_color, const size_t& background_color, T&& arg, U&&... args) { _output(std::cout, foreground_color, background_color, std::forward<T>(arg), std::forward<U>(args)...); }

	template <typename T, typename... U>
	static void output(const size_t& foreground_color, const size_t& background_color, const T& arg, const U&... args) { _output(std::cout, foreground_color, background_color, arg, args...); }

	template <typename T, typename... U>
	static void output_error(const size_t& foreground_color, const size_t& background_color, T&& arg, U&&... args) { _output(std::cerr, foreground_color, background_color, std::forward<T>(arg), std::forward<U>(args)...); }

	template <typename T, typename... U>
	static void output_error(const size_t& foreground_color, const size_t& background_color, const T& arg, const U&... args) { _output(std::cerr, foreground_color, background_color, arg, args...); }

	template <typename T, typename... U>
	static void output_log(const size_t& foreground_color, const size_t& background_color, T&& arg, U&&... args) { _output(std::clog, foreground_color, background_color, std::forward<T>(arg), std::forward<U>(args)...); }

	template <typename T, typename... U>
	static void output_log(const size_t& foreground_color, const size_t& background_color, const T& arg, const U&... args) { _output(std::clog, foreground_color, background_color, arg, args...); }

	template <typename T, typename... U>
	static void output_line(const size_t& foreground_color, const size_t& background_color, T&& arg, U&&... args) { _output(std::cout, foreground_color, background_color, std::forward<T>(arg), std::forward<U>(args)...); std::cout << std::endl; }

	template <typename T, typename... U>
	static void output_line(const size_t& foreground_color, const size_t& background_color, const T& arg, const U&... args) { _output(std::cout, foreground_color, background_color, arg, args...); std::cout << std::endl; }

	static void output_line() { std::cout << std::endl; }

	template <typename T>
	static void input(T& arg) { _input(arg); }

private:
	static std::string _current_working_directory() { char path[FILENAME_MAX]; CWD(path, sizeof(path)); path[sizeof(path) - 1] = NULL; return path; }

	static void _color(const size_t& foreground_color, const size_t& background_color) {
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)foreground_color | (WORD)AS_BACKGROUND(background_color));
#else
		std::cout << "\033[0;" << foreground_color << "m";
		std::cout << "\033[7;" << background_color << "m";
#endif
	}

	template <typename T>
	static void _input(T& arg) { std::cout << "> "; std::cin >> arg; }

	template<typename T>
	static void _output(std::ostream& stream, const size_t& foreground_color, const size_t& background_color, T&& arg) {
		_color(foreground_color, background_color);
		stream << std::forward<T>(arg);
	}

	template<typename T>
	static void _output(std::ostream& stream, const size_t& foreground_color, const size_t& background_color, const T& arg) {
		_color(foreground_color, background_color);
		stream << arg;
	}

	template <typename T, typename... U>
	static void _output(std::ostream& stream, const size_t& foreground_color, const size_t& background_color, T&& arg, U&&... args) {
		_output(stream, foreground_color, background_color, arg);
		if (sizeof...(args)) {
			_output(stream, foreground_color, background_color, std::forward<U>(args)...);
		}
	}

	template <typename T, typename... U>
	static void _output(std::ostream& stream, const size_t& foreground_color, const size_t& background_color, const T& arg, const U&... args) {
		_output(stream, foreground_color, background_color, arg);
		if (sizeof...(args)) {
			_output(stream, foreground_color, background_color, args...);
		}
	}
};

} // io
} // ftl