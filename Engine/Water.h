#pragma once

class Water : ITrigger {
public:
	Water(Object* water, CameraMove* camera) {
		_water = water;
		_water->setTriggerCallback(this);
		_camera = camera;
		_camera_up_level = 10;
	}

	int update(Scene* scene, Window* window) {
		if (!_water) return -1;

		double deltaTime = window->getDeltaTime();

		_water->getModel()->getMaterial()->getTransformTextures()->rotate(2* deltaTime, _water->getModel()->getMaterial()->getTransformTextures()->getDiraction().front);
		if (_camera) {
			glm::vec3 target_cam_pos = _camera->getTarget()->getTransform()->getPosition();
			glm::vec3 pos = _water->getTransform()->getPosition();

			if (target_cam_pos.y < pos.y) {
				_camera->setHeight(_camera_up_level);
			}
			else {
				_camera->setHeight(-1);
			}
		}
		return 0;
	}

	void trigger_callback(Object* object_b) override {
		object_b->getRigidbody()->addForce(-object_b->getRigidbody()->info.gravity * glm::vec3(2.f));
	}

private:
	Object* _water;
	CameraMove* _camera;
	float _camera_up_level;
};