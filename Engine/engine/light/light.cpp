#include "Light.h"

Light::Light() {
	_name = "Light";
	init();
}

Light::Light(const char* name) {
	this->_name = name;
	init();
}

Light::~Light() {
	if (_shadow)
		delete _shadow;
}

void Light::init() {
	_shadow = nullptr;
	_mode = DIRACTION_LIGHT_MODE;
	dirLight.direction = { 1, -1, 1 };
	dirLight.ambient = { 0.9f, 0.9f, 0.9f };
	dirLight.diffuse = { 0.1f, 0.1f, 0.1f };
	dirLight.specular = { .9f, .9f, 0.9f };
}

void Light::setMode(ModeLight mode) {
	this->_mode = mode;
}
const int Light::getMode() {
	return _mode;
}


const char* Light::getName() {
	return _name;
}

void Light::setShadowPS(ShadowPS* shadow) {
	_shadow = shadow;
}
ShadowPS* Light::getShadowPS() {
	return _shadow;
}