#include "Engine.h"

Engine::Engine() {
	_idActiveScene = -1;
	initEngine();
}



Engine::Engine(Scene* scenes[], int length) {
	if (scenes && length) {
		for (int i = 0; i < length; i++) {
			_scenes.insert({ i, scenes[i] });
		}
		_idActiveScene = 0;
	}
	else {
		_idActiveScene = -1;
	}
	initEngine();
}



void Engine::initEngine() {
	glewExperimental = GL_TRUE; 	// Подключение новейшей из доступных версий OpenGL
	if (glewInit())
		std::cerr << "ERROR: could not start GLEW\n";
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


Engine::~Engine() {
	for (_itScenes = _scenes.begin(); _itScenes != _scenes.end(); _itScenes++) {
		delete _itScenes->second;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}



int Engine::render(int width, int heigth, double deltaTime) {
	if (_idActiveScene >= 0 && _idActiveScene < _scenes.size()) {
		return _scenes.at(_idActiveScene)->render(width,heigth, deltaTime);
	}
	return -1;
}




void Engine::addScene(Scene* scene) {
	if (!scene) return;
	_scenes.insert({ _scenes.size(),scene });
	if (_idActiveScene < 0)_idActiveScene = 0;
}
void Engine::removeScene(int id_scene) {
	if (id_scene >= 0 && id_scene < _scenes.size()) {
		_scenes.erase(id_scene);
		if (id_scene == _idActiveScene)_idActiveScene = 0;
	}
}
void Engine::setActiveScene(int id_scene) {
	if (id_scene >= 0 && id_scene < _scenes.size()) {
		_idActiveScene = id_scene;
	}
}
int Engine::getLengthScenes() {
	return _scenes.size();
}
const int Engine::getIdActiveScene() {
	return _idActiveScene;
}



Scene* Engine::getScene(int id_scene) {
	if (id_scene >= 0 && id_scene < _scenes.size()) {
		return _scenes.at(id_scene);
	}
	return nullptr;
}
