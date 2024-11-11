#pragma once

class PlayerController: ITrigger{
public:
	PlayerController(Object* player, glm::vec3 max_position, Light* spot_light) {
		_player = player;
		_max_position = max_position;
		_spot = spot_light;
		_player->setTriggerCallback(this);

		_isSpotWork = false;
		_isGround = false;
		_isAnimFlag = false;

		_speed = 400.f;
		_rotate = 2.f;
		_jump = 50.f;
	}


	//Методы вызываются из сцены
	void key_callback(Window* window, int keys[KEYS_LENGTH], int _keyMods) {
		if (!_player) return;
		double deltaTime = window->getDeltaTime();

		glm::vec3 pos = _player->getTransform()->getPosition();
		if (pos.x < -_max_position.x) _player->getTransform()->move(_player->getTransform()->getDiraction().right * _speed);
		if (pos.x > _max_position.x) _player->getTransform()->move(_player->getTransform()->getDiraction().left * _speed);
		if (pos.y < -_max_position.y) _player->getTransform()->move(_player->getTransform()->getDiraction().up * _speed);
		if (pos.y > _max_position.y) _player->getTransform()->move(_player->getTransform()->getDiraction().bottom * _speed);
		if (pos.z < -_max_position.z) _player->getTransform()->move(_player->getTransform()->getDiraction().front * _speed);
		if (pos.z > _max_position.z) _player->getTransform()->move(_player->getTransform()->getDiraction().back * _speed);



		if (keys[GLFW_KEY_A])_player->getTransform()->rotate(_rotate, { 0,1,0 });
		if (keys[GLFW_KEY_D])_player->getTransform()->rotate(-_rotate, { 0,1,0 });
		if (keys[GLFW_KEY_W] && _isGround) {
			_player->getRigidbody()->addForce({ -_speed * deltaTime,0,0 });
			if (keys[GLFW_KEY_LEFT_SHIFT]) _player->getRigidbody()->addForce({ -_speed * deltaTime,0,0 });
		};
		if (keys[GLFW_KEY_S] && _isGround) {
			_player->getRigidbody()->addForce({ _speed * deltaTime,0,0 });
			if (keys[GLFW_KEY_LEFT_SHIFT]) _player->getRigidbody()->addForce({ _speed * deltaTime,0,0 });
		}
		if (keys[GLFW_KEY_W] == GLFW_RELEASE && keys[GLFW_KEY_S] || keys[GLFW_KEY_W] && keys[GLFW_KEY_S] == GLFW_RELEASE) {
			animationWalk();
			if (keys[GLFW_KEY_LEFT_SHIFT]) animationWalk();
		}else{
			_player->getTransform()->rotate(-_player->getTransform()->getRotationEuler().x, {1,0,0});
		}
		if (keys[GLFW_KEY_SPACE] && _isGround) _player->getRigidbody()->addForce({ 0,_jump,0 });



		//Работа с фонариком, проблемма в изначальном повороте модели
		if (keys[GLFW_KEY_1] == GLFW_PRESS) _isSpotWork = true;
		if (keys[GLFW_KEY_2] == GLFW_PRESS) _isSpotWork = false;
		if (_spot) {
			if (_isSpotWork) {
				_spot->spotLight.position = _player->getTransform()->getPosition() + glm::vec3({ 0,2,0 });
				_spot->spotLight.direction = glm::vec3({ -_player->getTransform()->getDiraction().right.x, _player->getTransform()->getDiraction().right.y - 1, _player->getTransform()->getDiraction().right.z });
			}
			else {
				_spot->spotLight.position = { -1000,-1000,-1000 };
				_spot->spotLight.direction = { -1000, -1001,-1000 };
			}
		}
	}

	int update(Scene* scene, Window* window) {
		_isGround = false;

		return 0;
	}

	
	void trigger_callback(Object* object_b) override{
		_isGround = true;
		//cout << object_b->getName() << endl;
	}


	Object* getObject() {
		return _player;
	}

private:
	Object* _player;
	glm::vec3 _max_position;
	Light* _spot;
	float _speed;
	float _rotate;
	float _jump;
	bool _isGround;
	bool _isSpotWork;




	bool _isAnimFlag;
	void animationWalk() {
		if (_player->getTransform()->getRotationEuler().x >= 15) {
			_isAnimFlag = true;
		}
		else if (_player->getTransform()->getRotationEuler().x <= -15) {
			_isAnimFlag = false;
		}

		if (_player->getTransform()->getRotationEuler().x < 15 && _isAnimFlag == false) {
			_player->getTransform()->rotate(_rotate, { 1,0,0 });
		}
		else if (_player->getTransform()->getRotationEuler().x > -15 && _isAnimFlag == true) {
			_player->getTransform()->rotate(-_rotate, { 1,0,0 });
		}

		if(_player->getTransform()->getRotationEuler().z)
			_player->getTransform()->rotate(-_player->getTransform()->getRotationEuler().z, { 0,0,1 });
	}


};