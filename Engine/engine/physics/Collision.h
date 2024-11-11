#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../component/Transform.h"
#include "../object/Model.h"
#include "../camera/Camera.h"
#include "Collider.h"


class Collision {
public:
	Collision(Transform* transform);
	~Collision();

	void setCollider(Collider* collider);
	void setDraw(bool draw);
	void setWork(bool work);
	Collider* getCollider();

	void update();
	int render(Camera* camera);
	CollisionPoints trigger(Collision* col_b);


private:
	bool _isDraw;
	bool _isWork;
	Collider* _collider;
};