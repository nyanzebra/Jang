#pragma once
	
#ifndef FTL_SETTINGS
#define FTL_SETTINGS

#include "utility.h"

#define CONFIGURATION jang::io::configurator::instance().settings()["graphics"]

#define DEFAULT_X_POSITION CONFIGURATION["position"]["x"].get<int>()	
#define DEFAULT_Y_POSITION CONFIGURATION["position"]["y"].get<int>()
#define DEFAULT_WINDOW_POSITION DEFAULT_X_POSITION, DEFAULT_Y_POSITION

#define DEFAULT_FOREGROUND_COLOR CONFIGURATION["color"]["foreground"].get<int>()
#define DEFAULT_BACKGROUND_COLOR CONFIGURATION["color"]["background"].get<int>()
#define DEFAULT_COLOR_SCHEME DEFAULT_FOREGROUND_COLOR, DEFAULT_BACKGROUND_COLOR

#endif
	 