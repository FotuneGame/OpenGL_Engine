#pragma once


class CameraMove {
public:
	CameraMove(Camera* cam, Object* target) {
		_angle = 0;
		_radius = 15;
		_speed = 5;
		_height = 3;
		_cam = cam;
		_target = target;
	}

	void key_callback(Window* window, int keys[KEYS_LENGTH], int _keyMods) {
		if (!_cam || !_target) return;

		double deltaTime = window->getDeltaTime();

		if (keys[GLFW_KEY_UP]) {
			if (_radius > 3) _radius -= deltaTime * _speed * 2;
		}
		if (keys[GLFW_KEY_DOWN]) {
			if (_radius < 20) _radius += deltaTime * _speed *2;
		}
		if (keys[GLFW_KEY_LEFT])_angle += deltaTime * _speed;
		if (keys[GLFW_KEY_RIGHT])_angle -= deltaTime * _speed;
		GLfloat camX = sin(_angle) * _radius;
		GLfloat camZ = cos(_angle) * _radius;
		_cam->getTransform()->lookAt(glm::vec3(camX, _height, camZ) + _target->getTransform()->getPosition(), _target->getTransform()->getPosition(), {0,1,0});
	}

	void setHeight(float height) {
		if (height >= 3)
			_height = height;
		else {
			_height = 3;
		}
	}

	Object* getTarget() {
		return _target;
	}

private:
	Object* _target;
	Camera* _cam;
	float _angle;
	float _radius;
	float _speed;
	float _height;
};

