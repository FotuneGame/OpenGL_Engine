#pragma once
#include <iostream>
#include <functional>
using namespace std;
#include "Model.h"
#include "../physics/Rigidbody.h"
#include "../physics/Collision.h"
#include "../component/Transform.h"
#include "../camera/Camera.h"

class ITrigger;

class Object {
public:
	Object();
	Object(const char* name);
	Object(Model* model,const char* name);
	Object(Model* model,Transform* transform, const char* name);
	~Object();

	int render(Camera* camera);
	void setTriggerCallback(ITrigger* trigger_callback);
	void trigger(Object* object_b);

	const char* getName();
	Model* getModel();
	Transform* getTransform();
	Collision* getCollision();
	Rigidbody* getRigidbody();

private:
	const char* _name;
	Model* _model;
	Transform* _transform;
	Collision* _collision;
	Rigidbody* _rigidbody;

	std::function<void(Object* object_b)> _trigger_callback;
};




class ITrigger {
public:
	virtual void trigger_callback(Object* object_b) = 0;
};