#pragma once

#include <vector>
#include <string>

class transforms {
public:
	static std::vector<std::string> tokenize(const std::string& value, const char& delimiter) {
		std::vector<std::string> result;

		std::string line;
		for (auto c : value) {
			if (c == delimiter) {
				result.push_back(line);
				line = "";
			} else {
				line += c;
			}
		}

		return result;
	}
};

