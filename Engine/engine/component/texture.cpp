#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture() {
	init();
}
Texture::Texture(const char* filename) {
	init();
	loadTexture(filename);
}
Texture::~Texture() {
	if(_data)
		stbi_image_free(_data);
}


void Texture::loadTexture(const char* filename) {
	if (!filename) return;
	_data = stbi_load(filename, &_width, &_height, &_nrChannels, STBI_default);
}



const int Texture::getWitdth() {
	return _width;
}
const int Texture::getHeight() {
	return _height;
}
const int Texture::getChannels() {
	return _nrChannels;
}
const unsigned char* Texture::getData() {
	return _data;
}


void Texture::init() {
	filter = GL_LINEAR;
	minimap = false;
	repaet = -1;
	borderColorClampMode = { 1,0,0 };
	horizontalRepeat = true;
}