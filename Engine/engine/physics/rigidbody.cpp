#include "Rigidbody.h"

Rigidbody::Rigidbody(Transform* transform) {
	_transform = transform;
	info.mass = 1.f;
	info.isStatic = true;
	info.isSupportReaction = true;
	info.staticFriction = 100.0f;
	info.dynamicFriction = 100.0f;
	info.restitution = .5f;
	info.gravity = glm::vec3(0, -9.8f, 0);
	_force = { 0,0,0 };
	_speed = { 0,0,0 };
}
Rigidbody::~Rigidbody(){}




void Rigidbody::useGravity(double deltaTime) {
	if (info.isStatic) return;

	_force += info.mass * info.gravity * glm::vec3(STEP_GRID_RIGIDBODY);
}

void Rigidbody::solveCollision(CollisionPoints collision, Rigidbody* rig_b, double deltaTime) {
	if (info.isStatic || !rig_b) return;

	/*
	//реакция опоры
	if (rig_b->info.isSupportReaction) {
		glm::vec3 correction = collision.normal * fmax(collision.depth + 0.4, 0.0f) / (info.mass + rig_b->info.mass);
		_force -= info.mass * correction * glm::vec3(STEP_GRID_RIGIDBODY);
	}
	//Импульс
	glm::vec3 aVel = _speed;
	glm::vec3 bVel = rig_b->getSpeed();
	glm::vec3 rVel = bVel - aVel;
	float  nSpd = glm::dot(rVel, collision.normal);

	// при <0 объекты будут притягиватся 
	if (nSpd >= 0) return;

	float e = (info.restitution) * (rig_b->info.restitution);
	float j = -(1.0f + e) * nSpd / (info.mass + rig_b->info.mass);
	glm::vec3 impluse = j * collision.normal;
	aVel -= impluse * info.mass;

	//Трение
	rVel = bVel - aVel;
	nSpd = glm::dot(rVel, collision.normal);

	glm::vec3 tangent = rVel - nSpd * collision.normal;

	if (glm::length(tangent) > 0.0001f) {
		tangent = glm::normalize(tangent);
	}

	float fVel = glm::dot(rVel, tangent);
	float mu = glm::vec2(info.staticFriction, rig_b->info.staticFriction).length();
	float f = -fVel / (info.mass + rig_b->info.mass);

	glm::vec3 friction;
	if (abs(f) < j * mu) {
		friction = f * tangent;
	}

	else {
		mu = glm::length(glm::vec2(info.dynamicFriction, rig_b->info.dynamicFriction));
		friction = -j * tangent * mu;
	}

	_force -= friction * info.mass * glm::vec3(STEP_GRID_RIGIDBODY);
	*/
	


	//Старый рабочий код
	
	//Трение и упроугость
	if (!rig_b->info.isStatic) {
		if (glm::length(_speed) > MIN_SPEED_RIGIDBODY) {
			_force += (-_speed) * info.mass * glm::vec3(STEP_GRID_RIGIDBODY * info.dynamicFriction);
		}
	}
	else {
		if (glm::length(_speed) > MIN_SPEED_RIGIDBODY) {
			_force += (-_speed) * info.mass * glm::vec3(STEP_GRID_RIGIDBODY * info.staticFriction);
		}
	}

	_force -= info.mass * collision.normal * glm::vec3(collision.depth * info.restitution) * _transform->getRotation();
	setSpeed(deltaTime);

}

void Rigidbody::render(double deltaTime) {
	if (!info.isStatic) {
		if (deltaTime >= MAX_FRAME_WAIT_RIGIDBODY)
			_transform->move(_speed * glm::normalize(glm::vec3(deltaTime * MAX_FRAME_WAIT_RIGIDBODY)));
		else
			_transform->move(_speed * glm::vec3(deltaTime));
	}
	setSpeed(deltaTime);
	_force = { 0,0,0 };
}

void Rigidbody::setSpeed(double deltaTime) {
	if (deltaTime >= MAX_FRAME_WAIT_RIGIDBODY)
		_speed += (_force / info.mass) * glm::normalize(glm::vec3(deltaTime * MAX_FRAME_WAIT_RIGIDBODY));
	else
		_speed += (_force / info.mass) * glm::vec3(deltaTime);
}


void Rigidbody::addForce(glm::vec3 force) {
	if (info.isStatic) return;

	_force += info.mass * force * glm::vec3(STEP_GRID_RIGIDBODY);
}

glm::vec3 Rigidbody::getForce() {
	return _force;
}
glm::vec3 Rigidbody::getSpeed() {
	return _speed;
}