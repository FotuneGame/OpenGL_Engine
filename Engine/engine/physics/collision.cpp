#include "Collision.h"

Collision::Collision(Transform* transform) {
	_isWork = false;
	_isDraw = false;
	_collider = new Collider(transform);
}
Collision::~Collision(){
	if (_collider)
		delete _collider;
}



void Collision::setCollider(Collider* collider) {
	_collider = collider;
}
void Collision::setDraw(bool draw) {
	_isDraw = draw;
}
void Collision::setWork(bool work) {
	_isWork = work;
}
Collider* Collision::getCollider() {
	return _collider;
}



void Collision::update() {
	if (_collider)
		_collider->updateTransformToVertices();
}
int Collision::render(Camera* camera) {
	if (!_isDraw) return 0;
	if (!_collider || !camera)return -2;

	return _collider->render(camera);
}




CollisionPoints Collision::trigger(Collision* col_b) {
	CollisionPoints answer;
	if (!_isWork || !_collider) return answer;
	if (this == col_b) return answer;


	answer = _collider->collision(col_b->getCollider());
	

	return answer;
}
