#pragma once

#include "utility.h"
#include "window/scene/renderable.h"

namespace ftl {
namespace studio {

enum class asset_type : char {
	kNONE = 0x0,
	kRENDERABLE = 0x1,
	kTEXTURE = 0x2,
	kSHADER = 0x4,
	kANIMATION = 0x8,
	kMATERIAL = 0x16
};



template<template<typename...> typename Allocator>
class asset {
public:
	typedef asset_type type;
	typedef graphics::renderable renderable_type;

	template<typename T, typename = std::enable_if_t <std::is_convertible<T, std::string>::value>>
	asset(const T& value) : _file(value) {}
	template<typename T, typename = std::enable_if_t <std::is_convertible<T, std::string>::value>>
	asset(T&& value) : _file(value) {}
	asset(const asset&) = default;
	asset(asset&&) = default;
	~asset() = default;


private:

	const bool _has(type type) { return _type & type; }
	const bool _has_renderable() { return _has(type::kRENDERABLE); }
	const bool _has_material() { return _has(type::kMATERIAL); }



private:
	io::file<byte> _file;
	type _type = type::kNONE;
};

}
}



