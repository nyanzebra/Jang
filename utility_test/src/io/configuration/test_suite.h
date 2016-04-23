#pragma once

#include "test.h"
#include "io/configurator.h"

TEST_SUITE(configuration) {
	TEST(load_json) {
		try {
			ftl::io::json::json json = ftl::io::configurator::instance().settings();
			std::string color = json["io"]["color"]["background"].get<std::string>();
			ASSERT_EQUAL(800, json["graphics"]["position"]["y"].get<int>());
		} catch (const ftl::io::json::json_exception& ex) {
			ftl::io::console::output_error(RED, BLACK, ex.what());
		}
	};
	TEST(store_json) {
		try {
			ftl::io::json::json json = ftl::io::configurator::instance().settings();
			ftl::io::console::output_line(WHITE, BLACK, json["test"]["color"]["failure"].get<std::string>());
			ftl::io::console::output_line(WHITE, BLACK, json.size());
			json["graphics"]["test"] = true;
		} catch (const ftl::io::json::json_exception& ex) {
			ftl::io::console::output_line(RED, BLACK, ex.what());
		}
	};
};
