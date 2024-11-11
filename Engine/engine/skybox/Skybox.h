#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "../object/Model.h"
#include "../component/Texture.h"
#include "../camera/Camera.h"
class Skybox {
public:
	Skybox();
	~Skybox();


	void loadCubeMap(vector <string> texturesArray);
	int render(Camera* camera);

	Model* getModel();
private:
	GLuint _cubeMap;
	GLuint _shaderProgramme;
	Model* _model;

	void useCubeMap();
};