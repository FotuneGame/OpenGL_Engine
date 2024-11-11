#pragma once
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

struct Direction {
	glm::vec3 up;
	glm::vec3 bottom;
	glm::vec3 left;
	glm::vec3 right;
	glm::vec3 front;
	glm::vec3 back;
};



class Transform {
public:
	Transform();
	Transform(glm::vec3 position, float angle, glm::vec3 axis, glm::vec3 size);
	Transform(glm::mat4 transformMatrix);
	~Transform();
	glm::mat4 getMatrix();
	void move(glm::vec3 addPosition);
	void rotate(float addAngle, glm::vec3 axis);
	void scale(glm::vec3 addScale);
	void lookAt(const glm::vec3 position, const  glm::vec3 target, const  glm::vec3 up);
	const glm::vec3 getPosition();
	const glm::vec3 getTarget();
	const glm::vec3 getScale();
	const glm::vec3 getRotationEuler();
	const glm::quat getRotation();
	const Direction getDiraction();
private:
	glm::mat4 _transformMatrix;
	glm::vec3 _target;
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::vec3 _angleEuler;
	glm::quat _angle;
	Direction _diraction;

	void calculateTransformVector();
	void calculateDiractionVector(glm::quat rotation);
};