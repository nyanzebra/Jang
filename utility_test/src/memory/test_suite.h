#pragma once

#include "utility.h"

TEST_SUITE(memory) {
	TEST(cache) {
		ftl::maths::precision<float, 1> value(3.14f);

		ftl::memory::cache<std::string, std::string, 1> cache;
		cache.insert("hello", "world");
		auto val = cache.at("hello");
		cache.insert("Robert", "Baldwin");
		try {
			val = cache.at("hello");
		} catch (std::out_of_range& err) {
			ftl::io::console::output_error(DEFAULT_COLOR_SCHEME, err.what());
		}
		ftl::io::console::output_line(DEFAULT_COLOR_SCHEME, val);
	};
};
