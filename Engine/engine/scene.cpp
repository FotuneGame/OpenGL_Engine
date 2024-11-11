#include "Scene.h"

Scene::Scene() {
	_name = "Test";	
	init();
}


Scene::Scene(const char* name) {
	this->_name = name;
	init();
}



Scene::Scene(Object* objects[], int length, const char* name) {
	this->_name = name;
	init();
	if (objects && length) {
		for (int i = 0; i < length; i++) {
			_objects.insert({i, objects[i]});
		}
	}
}

void Scene::init() {
	Camera* mainCamera = new Camera();
	_cameras.insert({ 0,mainCamera });
	Light* mainLight = new Light();
	_lights.insert({ 0,mainLight });
	_idActiveCamera = 0;
	_skybox = new Skybox();
	_shadow = nullptr;
}

Scene::~Scene() {
	for (_itObject = _objects.begin(); _itObject != _objects.end(); _itObject++) {
		delete _itObject -> second;
	}
	for (_itCamera = _cameras.begin(); _itCamera != _cameras.end(); _itCamera++) {
		delete _itCamera->second;
	}
	for (_itLigths = _lights.begin(); _itLigths != _lights.end(); _itLigths++) {
		delete _itLigths->second;
	}
	if (_skybox)
		delete _skybox;
	if (_shadow)
		delete _shadow;
}


int Scene::render(int width, int heigth, double deltaTime) {
	if (_idActiveCamera < 0 || _idActiveCamera >= _cameras.size() || _cameras.size()==0) return 2;
	int err = 0;

	//Рендер  карты теней
	if (_shadow)
		_shadow->render(_objects, _lights, _cameras.at(_idActiveCamera));

	//Рендер сцены с учетом света
	_cameras.at(_idActiveCamera)->clearZBuffer(width,heigth);
	unordered_map<int, Object*>::iterator _itObjectCollision;
	for (_itObject = _objects.begin(); _itObject != _objects.end() && !err; _itObject++) {


		//Учет света на объекте 
		for (_itLigths = _lights.begin(); _itLigths != _lights.end() && !err; _itLigths++) {
			_itObject->second->getModel()->getMaterial()->loadLight(_itLigths->second);
			if (_shadow)
				_itObject->second->getModel()->getMaterial()->loadShadowMap(_shadow->getShadowMap());
		}


		//Гравитация
		_itObject->second->getRigidbody()->useGravity(deltaTime);
		//Обновляем положение точек коллизии 
		_itObject->second->getCollision()->update();
		//Учёт коллизии 
		for (_itObjectCollision = _objects.begin(); _itObjectCollision != _objects.end() && !err; _itObjectCollision++) {
			if (_itObjectCollision == _itObject) continue;

			//Для двух статичных объектов не расчитываем коллизию !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if(_itObjectCollision->second->getRigidbody()->info.isStatic && _itObject->second->getRigidbody()->info.isStatic)continue;
			
			CollisionPoints answerDir = _itObject->second->getCollision()->trigger(_itObjectCollision->second->getCollision());
			if (answerDir.isCollision) {
				//Дёргаем триггер столкновения
				_itObject->second->trigger(_itObjectCollision->second);
				//Разрешение коллизии
				_itObject->second->getRigidbody()->solveCollision(answerDir, _itObjectCollision->second->getRigidbody(), deltaTime);
			}
		}
		//Применяем итогувую физику объекта
		_itObject->second->getRigidbody()->render(deltaTime);



		err = _itObject->second->render(_cameras.at(_idActiveCamera));
		err = _itObject->second->getCollision()->render(_cameras.at(_idActiveCamera));
	}
	err = _skybox->render(_cameras.at(_idActiveCamera));

	return err;
}



void Scene::addObject(Object* obj) {
	if (!obj) return;
	_objects.insert({ _objects.size(),obj });
}
void Scene::removeObject(int id_obj) {
	if (id_obj >= 0 && id_obj < _objects.size()) {
		_objects.erase(id_obj);
	}
}
int Scene::getLengthObjets() {
	return _objects.size();
}
Object* Scene::getObject(int id_obj) {
	if (id_obj >= 0 && id_obj < _objects.size()) {
		return _objects.at(id_obj);
	}
	return nullptr;
}



void Scene::addCamera(Camera* cam) {
	if (!cam) return;
	_cameras.insert({ _cameras.size(), cam });
}
void Scene::removeCamera(int id_cam) {
	if (id_cam >= 0 && id_cam < _cameras.size()) {
		_cameras.erase(id_cam);
	}
}
void Scene::setActiveCamera(int id_cam) {
	if (id_cam >= 0 && id_cam < _cameras.size()) {
		_idActiveCamera = id_cam;
	}
}
int Scene::getLengthCameras() {
	return _cameras.size();
}
Camera* Scene::getCamera(int id_cam) {
	if (id_cam >= 0 && id_cam < _cameras.size()) {
		return _cameras.at(id_cam);
	}
	return nullptr;
}



void Scene::addLight(Light* light) {
	if (!light) return;
	_lights.insert({ _lights.size(),light });

}
void Scene::removeLight(int id_light) {
	if (id_light >= 0 && id_light < _lights.size()) {
		_lights.erase(id_light);
	}
}
int Scene::getLengthLight() {
	return _lights.size();
}
Light* Scene::getLight(int id_light) {
	if (id_light >= 0 && id_light < _lights.size()) {
		return _lights.at(id_light);
	}
	return nullptr;
}


Skybox* Scene::getSkybox() {
	return _skybox;
}


const char* Scene::getName() {
	return _name;
}

void Scene::setShadow(Shadow* shadow) {
	if (_shadow)
		delete _shadow;
	_shadow = shadow;
}
