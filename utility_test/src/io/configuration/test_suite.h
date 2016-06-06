#pragma once

#include "test.h"
#include "io/configurator.h"

TEST_SUITE(configuration) {
	TEST(load_json) {
		try {
			jang::io::json::json json = jang::io::configurator::instance().settings();
			jang::string color = json["io"]["color"]["background"].get<jang::string>();
			ASSERT_EQUAL(800, json["graphics"]["position"]["y"].get<int>());
		} catch (const jang::io::json::json_exception& ex) {
			jang::io::console::output_error(RED, BLACK, ex.what());
		}
	};
	TEST(store_json) {
		try {
			jang::io::json::json json = jang::io::configurator::instance().settings();
			jang::io::console::output_line(WHITE, BLACK, json["test"]["color"]["failure"].get<jang::string>());
			jang::io::console::output_line(WHITE, BLACK, json.size());
			json["graphics"]["test"] = true;
		} catch (const jang::io::json::json_exception& ex) {
			jang::io::console::output_line(RED, BLACK, ex.what());
		}
	};
};
