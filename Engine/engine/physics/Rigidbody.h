#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Collision.h"
#include "../component/Transform.h"

#define STEP_GRID_RIGIDBODY 0.005
#define MAX_FRAME_WAIT_RIGIDBODY 0.02
#define MIN_SPEED_RIGIDBODY 0.02

struct RigidbodyInfo {
	float mass;
	bool isStatic; //нет дейтвия гравитации
	bool isSupportReaction; //Реакция опоры
	float staticFriction;  // Статичное трение
	float dynamicFriction; // динамичное трение
	float restitution;     // Эластичность столкновений (упругость)
	glm::vec3 gravity;
};

class Rigidbody {
public:
	Rigidbody(Transform* transform);
	~Rigidbody();

	void useGravity(double deltaTime);
	void solveCollision(CollisionPoints collision, Rigidbody* rig_b, double deltaTime);
	void render(double deltaTime);
	void addForce(glm::vec3 force);

	glm::vec3 getForce();
	glm::vec3 getSpeed();

	RigidbodyInfo info;
private:

	void setSpeed(double deltaTime);

	Transform* _transform;
	glm::vec3 _force;
	glm::vec3 _speed;
};