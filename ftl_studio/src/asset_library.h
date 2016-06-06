#pragma once

#include <map>

#include <utility>

#include "asset.h"
#include "asset_parser.h"

namespace jang {
namespace studio {

template<template <typename...> typename Allocator>
class asset_library {
public:
	asset_library() = default;
	~asset_library() = default;

	void load(const string& file_name) {
		asset<Allocator> file(file_name);

		_assets[file_name] = asset_parser(file);
	}

	asset<Allocator> search(const string& file_name) {
		if (_has(file_name)) {
			return _assets[file_name];
		}
		return asset<Allocator>();
	}

private:
	const bool _has(const string& file_name) { return _assets.find(file_name) != _assets.end(); }

private:
	map<string, asset<Allocator>, less<string>, Allocator> _assets;

	const unordered_map<string, asset_type> _file_types{
		{"png", asset_type::kTEXTURE} ,
		{"obj", asset_type::kRENDERABLE} ,
		{"mtl",asset_type::kMATERIAL}
	};
};

}
}
