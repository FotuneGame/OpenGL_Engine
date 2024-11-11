#pragma once
#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>


class ShadowPS {
public:
	ShadowPS(const char* vectorShader, const char* geometricShader, const char* fragmentShader);
	ShadowPS(const char* vectorShader, const char* geometricShader, const char* fragmentShader, int width, int heigth, float distance);
	~ShadowPS();

	int bind( glm::vec3 lightPos);
	GLuint getShadowCubeMap();
	glm::mat4 getShadowProjection();
	float getDistance();
	GLuint getShaderPrograme();
private:
	int _width;
	int _height;
	GLuint _fbo;
	GLuint _shaderProgramme;

	GLuint _shadowCubeMap;
	glm::mat4 _shadowProjection;
	float _distance;


	void loadShaders(const char* vectorShader, const char* geometricShader, const char* fragmentShader);
	static string readShaderFile(const char* filename);
	void init();
};