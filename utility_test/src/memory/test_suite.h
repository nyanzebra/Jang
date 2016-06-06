#pragma once

#include "utility.h"
#include "container/all.h"
#include "memory/cache.h"
#include "exception/exception.h"
#include "maths.h"

TEST_SUITE(memory) {
	TEST(cache) {
		jang::maths::precision<float, 1> value(3.14f);

		jang::memory::cache<jang::string, jang::string, 1> cache;
		cache.insert("hello", "world");
		auto val = cache.at("hello");
		cache.insert("Robert", "Baldwin");
		try {
			val = cache.at("hello");
		} catch (jang::out_of_range& err) {
			jang::io::console::output_error(DEFAULT_COLOR_SCHEME, err.what());
		}
		jang::io::console::output_line(DEFAULT_COLOR_SCHEME, val);
	};
};
