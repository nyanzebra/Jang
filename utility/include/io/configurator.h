#pragma once

#include "json/json.h"

namespace ftl {
namespace io {

#ifdef _WIN32
#define DEFAULT_CONFIG_FILE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/FTLGameEngine/util/include/configuration/configuration_windows_32.json"	
#elif _WIN64  
#define DEFAULT_CONFIG_FILE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/FTLGameEngine/util/include/configuration/configuration_windows_64.json"
#elif __APPLE__	
#define DEFAULT_CONFIG_FILE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/FTLGameEngine/util/include/configuration/configuration_apple.json"	
#elif __linux__	
#define DEFAULT_CONFIG_FILE "C:/Users/Robert/Documents/Visual Studio 2015/Projects/FTLGameEngine/util/include/configuration/configuration_linux.json"
#else
#endif // _WIN32

class configurator {
public:
	configurator(const configurator&) = delete;
	configurator(configurator&&) = delete;
	~configurator() { json::json::write(_json, DEFAULT_CONFIG_FILE); }

	static configurator& instance() {
		static configurator instance;
		return instance;
	}

	json::json& settings() { return _json; }

private:
	configurator() { _json = json::json::read(DEFAULT_CONFIG_FILE); }

private:
	json::json _json;
};

} // io
} // ftl
