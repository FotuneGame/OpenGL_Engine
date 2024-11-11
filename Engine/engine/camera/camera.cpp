#include "Camera.h"

Camera::Camera() {
	_name = "Camera Default";
	_transform = new Transform();
	_projectionMatrix = glm::mat4(1.0f);
	//setPerspective(120, 480, 480, 1.f, 100.);
	//setOrtographic(-1.f, 1.f, -1.f, 1.f, -0.01f, 100.f);
	_mode = GL_LESS;
}

Camera::Camera(const char* name) {
	this->_name = name;
	this->_transform = new Transform();
	this->_projectionMatrix = glm::mat4(1.0f);
	_mode = GL_LESS;
}

Camera::Camera(Transform* transform,const char* name) {
	this->_name = name;
	this->_transform = transform;
	this->_projectionMatrix = glm::mat4(1.0f);
	_mode = GL_LESS;
}

Camera::~Camera() {
	if(_transform)
		delete _transform;
}




void Camera::setRenderMode(GLuint mode_z_buffer) {
	_mode = mode_z_buffer;
}
void Camera::render() {
	if (_mode == -1) {
		glDisable(GL_DEPTH_TEST);
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(_mode);

	if(_mode!=GL_LESS)
		glClearDepth(0.0);
	else
		glClearDepth(GL_MAX);
}
void Camera::clearZBuffer(int width, int heigth) {
	glViewport(0, 0, width, heigth);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::setOrtographic(float left, float right, float buttom, float top, float in, float out) {
	_projectionMatrix = glm::ortho(
		left,
		right,
		buttom,
		top,
		in,
		out
	);
}
void Camera::setPerspective(float angle, int width, int height, float minDistance, float maxDistance) {
	_projectionMatrix = glm::perspective(
		glm::radians(angle),
		(float) width / height,
		minDistance,
		maxDistance
	);
}


const char* Camera::getName() {
	return _name;
}



glm::mat4& Camera::getProjection() {
	return _projectionMatrix;
}
Transform* Camera::getTransform() {
	return _transform;
}
