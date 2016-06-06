#pragma once

#include <dependencies.h>
#include "utility.h"

namespace jang {
namespace graphics {

inline void gl_error() {
	GLenum err = glGetError();
	if (err) {
		switch (err) {
		case GL_INVALID_ENUM:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tinvalid enum");
			break;
		case GL_INVALID_VALUE:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tinvalid value");
			break;
		case GL_INVALID_OPERATION:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tinvalid operation");
			break;
		default:
			io::console::output_error(DEFAULT_COLOR_SCHEME, "ERROR:\tunknown");
			break;
		}
	}
}

}
}

