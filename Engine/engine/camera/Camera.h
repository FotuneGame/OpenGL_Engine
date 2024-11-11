#pragma once
#include <iostream>
using namespace std;
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "../component/Transform.h"

class Camera {
public:
	Camera();
	Camera(const char* name);
	Camera(Transform* transform, const char* name);
	~Camera();
	
	void setRenderMode(GLuint mode_z_buffer);
	void render();
	void clearZBuffer(int width, int heigth);

	void setOrtographic(float left, float right, float buttom, float top, float in, float out);
	void setPerspective(float angle, int width, int height, float minDistance, float maxDistance);

	const char* getName();
	glm::mat4& getProjection();
	Transform* getTransform();
private:
	const char* _name;
	glm::mat4 _projectionMatrix;
	GLuint _mode;
	Transform* _transform;
};