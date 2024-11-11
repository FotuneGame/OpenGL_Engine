#include "Transform.h"

Transform::Transform() {
	_transformMatrix = glm::mat4(1.0f);
	_target = { 0,0,-1 };
	calculateTransformVector();
}



Transform::Transform(glm::vec3 position, float angle, glm::vec3 axis, glm::vec3 size) {
	_transformMatrix = glm::mat4(1.0f);
	_transformMatrix = glm::translate(_transformMatrix, position);
	_transformMatrix = glm::rotate(_transformMatrix, glm::radians(angle), axis);
	_transformMatrix = glm::scale(_transformMatrix, size);
	_target = { 0,0,-1 };
	calculateTransformVector();
}



Transform::Transform(glm::mat4 transformMatrix) {
	_transformMatrix = transformMatrix;
	_target = { 0,0,-1 };
	calculateTransformVector();
}



Transform::~Transform() {

}


//Postion модифиируется в началокоординат и фактическое смещение камеры только по z
void Transform::lookAt(const glm::vec3 position, const  glm::vec3 target, const  glm::vec3 up) {
	_transformMatrix = glm::lookAt(position, target, up);
	calculateTransformVector();
	_position = position;
	_target = target;
	_diraction.up = up;
}



glm::mat4 Transform::getMatrix() {
	return _transformMatrix;
}



void Transform::move(glm::vec3 addPosition) {
	_transformMatrix = glm::translate(_transformMatrix, addPosition);
	calculateTransformVector();
}



void Transform::rotate(float addAngle, glm::vec3 axis) {
	_transformMatrix = glm::rotate(_transformMatrix, glm::radians(addAngle) ,axis);
	calculateTransformVector();
}



void Transform::scale(glm::vec3 addScale) {
	_transformMatrix = glm::scale(_transformMatrix, addScale);
	calculateTransformVector();
}





const glm::vec3 Transform::getPosition() {
	return _position;
}
const glm::vec3 Transform::getTarget() {
	return _target;
}
const glm::vec3 Transform::getScale() {
	return _scale;
}

const glm::vec3 Transform::getRotationEuler() {
	return _angleEuler;
}

const glm::quat Transform::getRotation() {
	return _angle;
}


const Direction Transform::getDiraction() {
	return _diraction;
}



void Transform::calculateTransformVector() {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 positon;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(_transformMatrix, scale, rotation, positon, skew, perspective);

	_position = positon;
	_scale = scale;
	_angle = rotation;

	glm::vec3 rotation_euler;
	glm::extractEulerAngleYXZ(_transformMatrix, rotation_euler.y, rotation_euler.x, rotation_euler.z);
	_angleEuler = glm::degrees(rotation_euler);

	calculateDiractionVector(rotation);
}

void Transform::calculateDiractionVector(glm::quat rotation) {

	_diraction.front = glm::vec3({0,0,1}) * rotation;
	_diraction.right = glm::vec3({ 1,0,0 }) * rotation;
	_diraction.up = glm::vec3({ 0,1,0 }) * rotation;

	_diraction.back = -_diraction.front;
	_diraction.left = -_diraction.right;
	_diraction.bottom = -_diraction.up;
}