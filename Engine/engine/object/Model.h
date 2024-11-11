#pragma once
#include <iostream>
using namespace std;
#include "Mesh.h"
#include "Material.h"

class Model{
public:
	GLfloat sizeBrush;


	Model();
	Model(Mesh* mesh);
	~Model();


	int render(const glm::mat4 transformMatrix, const glm::mat4 transformCamera, const glm::mat4 projectionCamera, const glm::vec3 campos); //MODEL, VIEW, PROJECTION, Campos
	void setMode(GLuint mode);
	void setPolygonMode(GLuint type, GLuint mode);

	Material* getMaterial();
	Mesh* getMesh();

private:
	Material* _material;
	Mesh* _mesh;

	GLuint _mode;
	// 0- for  GL_FRONT, 1- for GL_BACK
	struct polygonRenderData {
		GLuint _type;
		GLuint _mode;
	} _polygonRenderData[2];

	void settingModeRender();
	void init();
};