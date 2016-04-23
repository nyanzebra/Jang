#pragma once

#include "utility.h"

TEST_SUITE(json) {
	TEST(null_covnersion) {
		try {
			ftl::io::json::json   json;
			json["null"] = nullptr;
			//ASSERT_NULLPTR((json["null"]));	
		} catch (std::exception& err) {
			ftl::io::console::output_error(RED, BLACK, err.what());
		}
	};
	TEST(bool_conversion) {
		try {
			ftl::io::json::json   json;
			json["bool"] = true;
			auto a = json["bool"];
			bool b = a;
			ASSERT_TRUE((b));
		} catch (std::exception& err) {
			ftl::io::console::output_error(RED, BLACK, err.what());
		}
	};
	TEST(number_conversion) {
		try {
			ftl::io::json::json   json;
			json["number"] = 1.0f;
			auto a = json["number"];
			float b = a;
			ASSERT_EQUAL(b, 1.0f);
		} catch (std::exception& err) {
			ftl::io::console::output_error(RED, BLACK, err.what());
		}
	};
	TEST(string_conversion) {
		try {
			ftl::io::json::json   json;
			json["string"] = "string";
			auto a = json["string"];
			std::string b = a;
		} catch (std::exception& err) {
			ftl::io::console::output_error(RED, BLACK, err.what());
		}
	};
	TEST(initialization_list_array) {
		try {
			ftl::io::json::json json;
			json["array"] = { 0, 1, 2 };
			ASSERT_EQUAL(json["array"][1].get<int>(), 1);
		} catch (std::exception& err) {
			ftl::io::console::output_error(RED, BLACK, err.what());
		}
	};
	TEST(initialization_list_object) {
		try {
			ftl::io::json::json json;
			json["array"] = { {"first", 0}, {"second", 1}, {"third", 2} };
			ASSERT_EQUAL(json["array"]["second"].get<int>(), 1);
		} catch (std::exception& err) {
			ftl::io::console::output_error(RED, BLACK, err.what());
		}
	};
};
