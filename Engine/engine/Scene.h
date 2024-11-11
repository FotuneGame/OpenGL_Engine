#pragma once
#include <unordered_map>
#include <iostream>
using namespace std;
#include "./object/Object.h"
#include "./camera/Camera.h"
#include "./light/Light.h"
#include "./skybox/Skybox.h"
#include "./light/Shadow.h"



class Scene {
public:
	Scene();
	Scene(const char* name);
	Scene(Object* objects[], int length,const char* name);
	~Scene();

	int render(int widht, int height, double deltaTime);


	void addObject(Object* obj);
	void removeObject(int id_obj);
	int getLengthObjets();
	Object* getObject(int id_obj);


	void addCamera(Camera* cam);
	void removeCamera(int id_cam);
	void setActiveCamera(int id_cam);
	int getLengthCameras();
	Camera* getCamera(int id_cam);


	void addLight(Light* light);
	void removeLight(int id_light);
	int getLengthLight();
	Light* getLight(int id_light);

	Skybox* getSkybox();
	void setShadow(Shadow* shadow);

	const char* getName();
private:
	const char* _name;
	int _idActiveCamera;
	unordered_map<int, Object*> _objects;
	unordered_map<int, Object*>::iterator _itObject;
	unordered_map<int, Camera*> _cameras;
	unordered_map<int, Camera*>::iterator _itCamera;
	unordered_map<int, Light*> _lights;
	unordered_map<int, Light*>::iterator _itLigths;
	Skybox* _skybox;
	Shadow* _shadow;

	void init();
};