#pragma once
#include <unordered_map>
#include <iostream>
using namespace std;
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Scene.h"

class Engine {
public:
	Engine();
	Engine(Scene* scenes[], int length);
	~Engine();

	int render(int width, int heigth, double deltaTime);

	void addScene(Scene* scene);
	void removeScene(int id_scene);
	void setActiveScene(int id_scene);
	int getLengthScenes();
	const int getIdActiveScene();
	Scene* getScene(int id_scene);
private:
	int _idActiveScene;
	unordered_map<int,Scene*> _scenes;
	unordered_map<int, Scene*>::iterator _itScenes;
	void initEngine();
};
