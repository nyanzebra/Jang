#include <FreeImage/FreeImage.h>

#include "..\..\..\..\..\include\window\scene\renderable\material\texture.h"

namespace jang {
namespace graphics {

void texture::bind() {
	if (!_is_bound) {
		glBindTexture(GL_TEXTURE_2D, _id);
		_is_bound = true;
	}
}

void texture::unbind() {
	if (_is_bound) {
		glDeleteTextures(1, &_id);
		_is_bound = false;
	}
}

template <typename T, typename = std::enable_if_type<std::is_convertible<T, jang::string>::value>>
const bool texture::load(T file_name, bool generate_mipmap) {
	_file_name = jang::string(file_name);

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib = nullptr;

	fif = FreeImage_GetFileType(_file_name.c_str(), 0);

	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(_file_name.c_str());
	}
	if (fif == FIF_UNKNOWN) {
		return false;
	}

	if (FreeImage_FIFSupportsReading(fif)) {
		dib = FreeImage_Load(fif, _file_name.c_str());
	}
	if (!dib) {
		return false;
	}

	_width = FreeImage_GetWidth(dib);
	_height = FreeImage_GetHeight(dib);
	_bits_per_pixel = FreeImage_GetBPP(dib);
	int size = _width * _height * (_bits_per_pixel / 8);
	BYTE* pixels = FreeImage_GetBits(dib);
	_data = new BYTE[size];
	memcpy(_data, pixels, size);

	glGenTextures(1, &_id);
	configure(dimension, aliasing);
	FreeImage_Unload(dib);
	GLerror::errorCheck();

	return true;
}

}
}

