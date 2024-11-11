#pragma once
#include <iostream>
using namespace std;
#include <gl/glew.h>
#include <unordered_map>
#include "../object/Object.h"
#include "../camera/Camera.h"

class Shadow {
public:
	Shadow(const char* vectorShader, const char* fragmentShader);
	Shadow(const char* vectorShader, const char* fragmentShader,int width, int heigth, float distanceDir);
	~Shadow();
	
	int render(unordered_map<int, Object*> objects, unordered_map<int, Light*> lights, Camera* camera);
	void renderPS(unordered_map<int, Object*> objects, Light* light, Camera* camera);
	void renderDir(unordered_map<int, Object*> objects, Light* dirLight, Camera* camera);
	GLuint getShadowMap();
private:
	int _width;
	int _height;
	float _distanceDir;
	GLuint _fbo;
	GLuint _shadowMap;
	GLuint _shaderProgramme;


	void loadShaders(const char* vectorShader, const char* fragmentShader);
	static string readShaderFile(const char* filename);
	void init();
};