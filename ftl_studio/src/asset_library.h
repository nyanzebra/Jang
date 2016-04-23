#pragma once

#include <map>

#include "utility.h"

#include "asset.h"
#include "asset_parser.h"

namespace ftl {
namespace studio {

template<template<typename...> typename Allocator>
class asset_library {
public:
	asset_library() = default;
	~asset_library() = default;

	void load(const std::string& file_name) {
		asset<Allocator> file(file_name);

		_assets[file_name] = asset_parser(file);
	}

	asset<Allocator> search(const std::string& file_name) {
		if (_has(file_name)) {
			return _assets[file_name];
		}
		return asset<Allocator>();
	}

private:
	const bool _has(const std::string& file_name) { return _assets.find(file_name) != _assets.end(); }

private:
	std::map<std::string, asset<Allocator>, std::less<std::string>, Allocator> _assets;

	const std::unordered_map<std::string, asset_type> _file_types{
		{"png", asset_type::kTEXTURE} ,
		{"obj", asset_type::kRENDERABLE} ,
		{"mtl",asset_type::kMATERIAL}
	};
};

}
}
