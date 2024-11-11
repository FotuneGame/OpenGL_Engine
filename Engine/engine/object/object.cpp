#include "Object.h"

Object::Object() {
	_name = "Test object";
	_model = new Model();
	_transform = new Transform();
	_collision = new Collision(_transform);
	_rigidbody = new Rigidbody(_transform);
}


Object::Object(const char* name) {
	this->_name = name;
	_model = new Model();
	_transform = new Transform();
	_collision = new Collision(_transform);
	_rigidbody = new Rigidbody(_transform);
}

Object::Object(Model* model,const char* name) {
	this->_name = name;
	this->_model = model;
	_transform = new Transform();
	_collision = new Collision(_transform);
	_rigidbody = new Rigidbody(_transform);
}

Object::Object(Model* model, Transform* transform, const char* name) {
	this->_name = name;
	this->_model = model;
	this->_transform = transform;
	_collision = new Collision(_transform);
	_rigidbody = new Rigidbody(_transform);
}


Object::~Object() {
	if(_model)
		delete _model;
	if(_transform)
		delete _transform;
	if (_collision)
		delete _collision;
	if (_rigidbody)
		delete _rigidbody;
}



int Object::render(Camera* camera) {
	if(!_model || !_transform || !camera)return -2;
	camera->render();
	return _model->render(_transform->getMatrix(), camera->getTransform()->getMatrix(), camera->getProjection(), camera->getTransform()->getPosition());
}
void Object::setTriggerCallback(ITrigger* trigger_callback) {
	_trigger_callback = [trigger_callback](Object* object_b) {trigger_callback->trigger_callback(object_b); };
}
void Object::trigger(Object* object_b) {
	if (_trigger_callback)
		_trigger_callback(object_b);
}


const char* Object::getName() {
	return _name;
}

Model* Object::getModel() {
	return _model;
}
Transform* Object::getTransform() {
	return _transform;
}

Collision* Object::getCollision() {
	return _collision;
}

Rigidbody* Object::getRigidbody() {
	return _rigidbody;
}