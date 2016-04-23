#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <ciso646>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <regex>

#include "../file.h"
#include "../../string/transforms.h"

namespace ftl {
namespace io {
namespace json {

template<typename T>
struct has_const_iterator {
private:
	template<typename C> static char test(typename C::const_iterator*);
	template<typename C> static int test(...);
public:
	enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

class json_exception : std::exception {
public:
	json_exception(std::string method, std::string type) : _method(method), _type(type) {}
	virtual const char* what() const throw() { console::output_line(WHITE, BLACK, _type); return std::string("ERROR:\tcannot use " + _method + " on type " + _type).c_str(); }
private:
	std::string _method;
	std::string _type;
};

template<
	template<typename Key, typename T, typename... Args> class Object = std::map,
	template<typename T, typename... Args>   class Array = std::vector
>
class _json_ {
public:
	typedef _json_<Object, Array> _json_t;
	typedef _json_ value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type&& move_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::size_t size_type;

	typedef Object<std::string, value_type>  object_type;
	typedef std::string key_type;
	typedef Array<value_type> array_type;

	enum class json_type : unsigned char {
		kNULL,
		kOBJECT,
		kARRAY,
		kSTRING,
		kBOOL,
		kNUMBER
	};

	typedef json_type type;

	_json_() noexcept : _type(type::kNULL) {}
	_json_(std::nullptr_t) noexcept : _type(type::kNULL) {}
	_json_(const object_type& value) :_type(type::kOBJECT), _object(value) {}
	_json_(const array_type& value) : _type(type::kARRAY), _array(value) {}
	template<typename T, typename = std::enable_if_t<std::is_constructible<value_type, typename T::value_type>::value>>
	_json_(const T& value) : _type(type::kARRAY) {
		_array = array_type(std::begin(value), std::end(value));
	}
	_json_(const std::string& value) : _type(type::kSTRING), _string(value) {}
	_json_(const char* value) : _type(type::kSTRING), _string(value) {}
	_json_(bool value) : _type(type::kBOOL), _bool(value) {}
	template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
	_json_(const T value) : _type(type::kNUMBER), _number(value) {}
	_json_(std::initializer_list<value_type> init) {
		bool is_object = true;
		for (const auto& it : init) {
			if (!it.is_array() || it.size() != 2 || !it[0].is_string()) {
				is_object = false;
				break;
			}
		}
		if (is_object) {
			_type = type::kOBJECT;
			for (auto& it : init) {
				_object.emplace(std::move(it[0]._string), std::move(it[1]));
			}
		} else {
			_type = type::kARRAY;
			_array = std::move(init);
		}
	}
	_json_(const value_type& other) : _type(other._type), _object(other._object), _array(other._array), _string(other._string), _number(other._number), _bool(other._bool) {}
	_json_(value_type&& other) : _type(std::move(other._type)), _object(std::move(other._object)), _array(std::move(other._array)), _string(std::move(other._string)), _number(std::move(other._number)), _bool(std::move(other._bool)) { other._type = type::kNULL; }
	~_json_() = default;

	reference operator=(const value_type& other) noexcept {
		_type = other._type;
		_object = other._object;
		_array = other._array;
		_string = other._string;
		_number = other._number;
		_bool = other._bool;
		return *this;
	}

	reference operator=(value_type&& other) noexcept {
		_type = std::move(other._type);
		_object = std::move(other._object);
		_array = std::move(other._array);
		_string = std::move(other._string);
		_number = std::move(other._number);
		_bool = std::move(other._bool);
		other._type = type::kNULL;
		return *this;
	}

	static value_type read(file<>&& file) {
		file.open(io::file<>::mode::kREAD);
		std::string data;
		file.read(data.begin(), data.end());
		file.close();

		std::vector<std::string> lines = transforms::tokenize(data, '\n');
		size_t begin = 1;
		size_t end = lines.size() - 1;
		return _create_json_object(lines, begin, end);
	}
																										 
	static void write(const value_type& root, file<>&& file) {
		if (file.open(io::file<>::mode::kWRITE)) {
			std::string buffer = "{\n" + _to_string(root);
			console::output_line(WHITE, BLACK, buffer);
			file.write(buffer.begin(), buffer.end());
			file.close();
		}
	}		   

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, key_type>::value>>
	reference at(const T& key) { return _at(key); }

	reference at(const size_type& pos) { return  _at(pos); }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, key_type>::value>>
	const_reference at(const T& key) const { return _at(key); }

	const_reference at(const size_type& pos) const { return  _at(pos); }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, key_type>::value>>
	reference operator[] (const T& key) { return  at(key); }

	reference operator[] (const size_type& pos) { return  at(pos); }

	template<typename T, typename = std::enable_if_t<std::is_convertible<T, key_type>::value>>
	const_reference operator[] (const T& key) const { return  at(key); }

	const_reference operator[] (const size_type& pos) const { return  at(pos); }

	template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
	T get() const { return _get(static_cast<T*>(nullptr)); }

	template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
	T get() noexcept { return get_ptr<T>(); }

	template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
	T get_ptr() noexcept { return _get_ptr(static_cast<T>(nullptr)); }

	template<typename T,
		typename = std::enable_if_t <
		!std::is_pointer<T>::value &&
		!std::is_same<T, typename std::string::value_type>::value
#ifndef _MSC_VER 
		&& !std::is_same<T, std::initializer_list<typename std::string::value_type>>::value
#endif>
	> >
		operator T() const { return get<T>(); }

	void push_back(value_type&& value) {
		if (!(is_null() || is_array())) {
			throw json_exception(__func__, json_type());
		}
		if (is_null()) {
			_type = type::kARRAY;
		}
		_array.push_back(std::move(value));
		value._type = type::kNULL;
	}

	void push_back(const value_type& value) {
		if (!(is_null() || is_array())) {
			throw json_exception(__func__, json_type());
		}
		if (is_null()) {
			_type = type::kARRAY;
		}
		_array.push_back(value);
	}

	void push_back(const typename object_type::value_type& value) {
		if (!(is_null() || is_object())) {
			throw json_exception(__func__, json_type());
		}
		if (is_null()) {
			_type = type::kOBJECT;
		}
		_object.insert(value);
	}

	operator type() const noexcept { return _type; }

	const size_t size() const { return _size(); }

	const bool is_null() const noexcept { return _type == type::kNULL; }

	const bool is_bool()const noexcept { return _type == type::kBOOL; }

	const bool is_string()const noexcept { return _type == type::kSTRING; }

	const bool is_number()const noexcept { return _type == type::kNUMBER; }

	const bool is_fundamental()const noexcept { return is_number() || is_bool() || is_string() || is_null(); }

	const bool is_array()const noexcept { return _type == type::kARRAY; }

	const bool is_object()const noexcept { return _type == type::kOBJECT; }

	const bool is_constructible() const noexcept { return is_object() || is_array() || is_string(); }

	const bool is_container() const noexcept { return is_object() || is_array(); }

	const std::string json_type() const noexcept {
		switch (_type) {
		case type::kNULL:
			return "NULL";
		case type::kBOOL:
			return "bool";
		case type::kNUMBER:
			return "number";
		case type::kSTRING:
			return "string";
		case type::kARRAY:
			return "array";
		case type::kOBJECT:
			return "object";
		default:
			return "?";
		}
	}

private:

	object_type _get(object_type*) const {
		if (is_object()) {
			return _object
		}
		throw json_exception(__func__, json_type());
	}

	template<typename T, typename = std::enable_if_t<std::is_convertible<_json_t, T>::value &&!std::is_same<_json_t, T>::value>>
	std::vector<T> _get(std::vector<T>*) const {
		if (is_array()) {
			std::vector<T> result;
			std::transform(
				_array().begin(),
				_array().end(),
				std::inserter(result, result.end()),
				[](value_type x) {
				return x.get< T>();
			});
			return result;
		}
		throw json_exception(__func__, json_type());
	}

	array_type _get(array_type*) const {
		if (is_array()) {
			return _array;
		}
		throw json_exception(__func__, json_type());
	}

	std::string _get(std::string*)const {
		if (is_string()) {
			return _string;
		}
		throw json_exception(__func__, json_type());
	}

	template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
	T _get(T*) const {
		if (is_number()) {
			return static_cast<T>(_number);
		}
		throw json_exception(__func__, json_type());
	}

	bool _get(bool*) const {
		if (is_bool()) {
			return  _bool;
		}
		throw json_exception(__func__, json_type());
	}

	object_type* _get_ptr(object_type*) noexcept { return is_object() ? &_object : nullptr; }

	array_type* _get_ptr(array_type*) noexcept { return is_array() ? &_array : nullptr; }

	std::string* _get_ptr(std::string*) noexcept { return is_string() ? &_string : nullptr; }

	bool* _get_ptr(bool*) noexcept { return is_bool() ? &_bool : nullptr; }

	double* _get_ptr(double*) noexcept { return is_number() ? &_number : nullptr; }

	reference _at(const key_type& key) {
		if (is_null()) {
			_type = type::kOBJECT;
			_object = object_type();
		}
		if (is_object()) {
			try {
				return _object[key];
			} catch (std::out_of_range& err) {
				throw err;
			}
		}
		throw json_exception(__func__, json_type());
	}

	reference _at(const size_type& pos) {
		if (is_null()) {
			_type = type::kARRAY;
			_array = array_type();
		}
		if (is_array()) {
			try {
				return _array[pos];
			} catch (std::out_of_range& err) {
				throw err;
			}
		}
		throw json_exception(__func__, json_type());
	}

	const_reference _at(const key_type& key) const {
		if (is_object()) {
			try {
				return _object[key];
			} catch (std::out_of_range& err) {
				throw err;
			}
		}
		throw json_exception(__func__, json_type());
	}

	const_reference _at(const size_type& pos) const {
		if (is_array()) {
			try {
				return _array[pos];
			} catch (std::out_of_range& err) {
				throw err;
			}
		}
		throw json_exception(__func__, json_type());
	}

	const size_t _size() const noexcept {
		switch (_type) {
		case type::kBOOL:
		case type::kNUMBER:
			return 1;
		case type::kSTRING:
			return _string.size();
		case type::kARRAY:
			return _array.size();
		case type::kOBJECT:
			return _object.size();
		default:
			return 0;
		}
	}

	const std::string _closing(const char& closing_container, const size_t& tabs, const bool& should_skip = false) const {
		std::string result = std::string(1, closing_container);
		result.insert(0, _tabs(tabs));
		return result;
	}

	const std::string _tabs(const size_t& tabs) const {
		std::string result = "";
		for (size_t i = 0; i < tabs; ++i) {
			result += "\t";
		}
		return result;
	}

	static std::pair<std::string, std::string> _get_key_value(const std::string& line) {
		std::string key = (_has_key(line)) ? line.substr(1, line.find(':') - 2) : "";
		std::string value = line.substr(key.size() + 3);
		return std::make_pair(key, value);
	}

	const std::string _format_key(const std::string& key) const { return "\"" + key + "\":"; }

	static value_type _create_json_object(const std::vector<std::string>& data, size_t& begin, size_t& end) {
		value_type root;
		while (begin != end) {
			std::string line = data[begin];
			_trim_format_chars(line);
			std::pair<std::string, std::string> key_value_pair = _get_key_value(line);

			std::string key = key_value_pair.first;
			std::string value = key_value_pair.second;
			if (!key_value_pair.first.empty()) {
				if ((value.compare("true") == 0) || (value.compare("false") == 0)) {
					root[key] = value.compare("true") == 0 ? true : false;
				} else if (std::regex_match(value, std::regex("^-?\\d+"))) {
					root[key] = stod(value);
				} else if ((value[0] == value[value.size() - 1]) && value[0] == '"') {
					root[key] = value.substr(1, value.size() - 2);
				} else	if (value[0] == '[') {
					root[key] = _create_json_array(data, begin, end);
				} else if (value[0] == '{') {
					begin++;
					root[key] = _create_json_object(data, begin, end);
				}
			}
			if (begin + 1 < end) {
				line = data[++begin];
				_trim_format_chars(line);
			}
			if (line[0] == '}') {
				break;
			}
		}

		return root;
	}

	static value_type _create_json_array(const std::vector<std::string>& data, size_t& begin, size_t& end) {
		value_type result;
		while (begin != end) {
			std::string line = data[begin];
			_trim_format_chars(line);
			value_type item;
			if ((line.compare("true") == 0) || (line.compare("false") == 0)) {
				item = (!line.compare("true")) ? true : false;
			} else if (std::regex_match(line, std::regex("^-?\\d+"))) {
				item = stod(line);
			} else if ((line[0] == line[line.size() - 1]) && line[0] == '"') {
				item = line;
			} else if (line[0] == '[') {
				item = _create_json_array(data, begin, end);
			} else if (line[0] == '{') {
				item = _create_json_object(data, begin, end);
			}
			begin++;

			if (line[0] == '{') {
				result.push_back(std::make_pair("", item));	 // push_back is having a deduction issue
			} else {
				result.push_back(item);
			}

			line = data[begin];
			_trim_format_chars(line);
			if (line[0] == ']') {
				begin++;
				break;
			}
		}

		return result;
	}

	static void _trim_format_chars(std::string& value) {
		size_t pos = value.find("\"");
		pos = (pos == std::string::npos) ? 0 : pos;
		value = value.substr(pos, value.size() - pos);
		value.erase(std::remove(value.begin(), value.end(), '\t'), value.end());
		value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
		value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
		bool can_have_whitespace = false;
		std::string result = "";
		for (auto c : value) {
			if (c == '\"') {
				can_have_whitespace = !can_have_whitespace;
			}
			if (c == ' ' && can_have_whitespace) {
				result += c;
			}
			if (c != ' ') {
				result += c;
			}
		}
		if (result[result.size() - 1] == ',') {
			result.pop_back();
		}
		value = result;
	}

	static std::string _to_string(const value_type& root, int tabs = 0) {
		std::string result;
		if (root.is_fundamental()) {
			if (root.is_string()) {
				result += root._string;
				result.insert(0, "\"");
				result.append("\"");
			} else if (root.is_number()) {
				result += std::to_string(root._number);
			} else if (root.is_bool()) {
				result += (root._bool) ? "true" : "false";
			}
			console::output_line(WHITE, BLACK, result);
			return result;
		}
		if (root.is_array()) {
			for (auto it = root._array.begin(); it != root._array.end(); ++it) {
				result += _to_string(*it, tabs);

				if (std::next(it) != root._array.end()) {
					result += ',';
				}

				result += '\n';
			}

			console::output_line(WHITE, BLACK, result + root._closing(']', tabs));
			return result + root._closing(']', tabs);
		}
		if (root.is_object()) {
			for (auto it = root._object.begin(); it != root._object.end(); ++it) {
				auto key = it->first;
				result.append(root._tabs(tabs));
				result += root._format_key(key);

				if (it->second.is_object()) {
					result += "{\n" + _to_string(it->second, tabs + 1);
				} else if (it->second.is_array()) {
					result += "[\n" + _to_string(it->second, tabs + 1);
				} else {
					result += _to_string(it->second);
				}

				if (std::next(it) != root._object.end()) {
					result += ',';
				}

				result += '\n';
			}

			console::output_line(WHITE, BLACK, result + root._closing('}', tabs));
			return result + root._closing('}', tabs);
		}
		return "";
	}

	static bool _has_key(const std::string& value) { return value[0] == '"' && value.find(':') != std::string::npos; }

private:
	type _type = type::kNULL;
	object_type _object;
	array_type _array;
	std::string _string = "";
	double _number = 0.0;
	bool _bool = false;
};

typedef _json_<> json;

} // json
} // io
} // ftl
