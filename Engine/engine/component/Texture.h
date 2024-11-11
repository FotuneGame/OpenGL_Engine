#pragma once
#include "Transform.h"
#include <glm/glm.hpp>
#include <GL/glew.h>


class Texture {
public:
	Texture();
	Texture(const char* filename);
	~Texture();
	void loadTexture(const char* filename);
	const int getWitdth();
	const int getHeight();
	const int getChannels();
	const unsigned char* getData();

	//GL_NEAREST | GL_LINEAR 
	GLuint filter;
	bool minimap;

	//GL_REPEAT | GL_MIRRORED_REPEAT |  GL_CLAMP_TP_EDGE | GL_CLAMP_TO_BORDER
	GLuint repaet;
	glm::vec3 borderColorClampMode;
	bool horizontalRepeat;

private:
	int _width;
	int _height;
	int _nrChannels;
	unsigned char* _data;
	void init();
};
