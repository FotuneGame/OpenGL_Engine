#pragma once

#include "LoaderObjects.h"
#include "CameraMove.h"
#include "PlayerCotroller.h"
#include "Water.h"

class SceneGame {
public:

	SceneGame(Engine* engine, Scene* scene, Window* window) {
		SetSkybox(scene);
		InitLight(scene);
		InitCamera(engine, window);
		LoadObjects(scene);
		cout << "Load Sas game" << endl;
	}

	~SceneGame() {
		if (_camera)
			delete _camera;
		if (_player)
			delete _player;
		if (_water)
			delete _water;
	}

	void key_callback(Scene* scene, Window* window, int keys[KEYS_LENGTH], int _keyMods){
		if (keys[GLFW_KEY_ESCAPE]) window->close();
		if (_camera)
			_camera->key_callback(window, keys, _keyMods);
		if (_player)
			_player->key_callback(window, keys, _keyMods);

	}

	int update(Scene* scene, Window* window){
		int err = 0;
		if (_player)
			err =_player->update(scene, window);
		if (_water)
			err =_water->update(scene, window);
		return err;
	}

private:
	PlayerController* _player;
	CameraMove* _camera;
	Water* _water;

	void SetSkybox(Scene* scene) {
		Skybox* sky = scene->getSkybox();

		vector <string> textures = {
			"./texture/skybox3/right.png",
			"./texture/skybox3/left.png",
			"./texture/skybox3/top.png",
			"./texture/skybox3/bottom.png",
			"./texture/skybox3/front.png",
			"./texture/skybox3/back.png"
		};
		sky->loadCubeMap(textures);
		sky->getModel()->getMesh()->loadModelFromFile("./model/Cube.obj");
		sky->getModel()->getMaterial()->loadShaders("./engine/shader/vs_skybox.vert", "./engine/shader/fs_skybox.frag");
		sky->getModel()->setMode(GL_TRIANGLES);

	}

	void InitLight(Scene* scene) {
		//Источники света
		Light* mainLight = scene->getLight(0);
		mainLight->dirLight.ambient = { .5,.5,.5 };
		mainLight->dirLight.diffuse = { 0.2,0.2,0.2 };
		mainLight->dirLight.direction = { -1,-1,-1 };
		mainLight->dirLight.specular = { 0.2,0.2,0.2 };

		//Настрока теней
		Shadow* shadow = new Shadow("./engine/shader/vs_shadowMap.vert", "./engine/shader/fs_shadowMap.frag");
		scene->setShadow(shadow);

		Light* spot_light = new Light();
		spot_light->setMode(SPOT_LIGHT_MODE);
		spot_light->spotLight.position = { 0,0,0 };
		spot_light->spotLight.direction = { 0,0,-1 };
		spot_light->spotLight.cutOff = cos(glm::radians(0.));
		spot_light->spotLight.outerCutOff = cos(glm::radians(60.));
		spot_light->spotLight.constant = 1;
		spot_light->spotLight.linear = 0.0;
		spot_light->spotLight.quadratic = 0.0;
		spot_light->spotLight.ambient = { 1,1,1 };
		spot_light->spotLight.diffuse = { 0.5,0.5,0.5 };
		spot_light->spotLight.specular = { 0.5,0.5 ,0.5 };
		scene->addLight(spot_light);

		//Настройка теней для spot_light
		ShadowPS* shadow_spot = new ShadowPS("./engine/shader/vs_shadowCubeMap.vert", "./engine/shader/geom_shadowCubeMap.geom", "./engine/shader/fs_shadowCubeMap.frag");
		spot_light->setShadowPS(shadow_spot);

		
		/*
		//Свет из дома
		Light* point_light = new Light();
		point_light->setMode(POINT_LIGHT_MODE);
		point_light->pointLight.ambient = { 1,1,1 };
		point_light->pointLight.diffuse = { 0.5,0.5,0.5 };
		point_light->pointLight.specular = { 0.5,0.5 ,0.5 };
		point_light->pointLight.constant = 1;
		point_light->pointLight.linear = 0.14;
		point_light->pointLight.quadratic = 0.07;
		point_light->pointLight.position = { -15,-1,-10 };
		scene->addLight(point_light);

		//Настройка под теней для point_light
		ShadowPS* shadow_point = new ShadowPS("./engine/shader/vs_shadowCubeMap.vert", "./engine/shader/geom_shadowCubeMap.geom", "./engine/shader/fs_shadowCubeMap.frag");
		point_light->setShadowPS(shadow_point);
		*/
	}

	void LoadObjects(Scene* scene) {

		//Настройка текстур
		Texture* texture_box = new Texture("./texture/clear/box.jpg");
		Texture* texture_box_beer = new Texture("./texture/clear/box_beer.png");
		Texture* texture_ball = new Texture("./texture/clear/ball.jpg");


		//Игрок
		scene->addObject(ObjCapy(GL_TRIANGLES, { 0,0,0 }));
		//Вода
		scene->addObject(ObjWater({ 0,-3,0 }, 1));
		//Игравая площадка
		scene->addObject(ObjTerrain({ 0,-2,0 }, 0.5));

		_player = new PlayerController(scene->getObject(0), { 60,100,60 }, scene->getLight(1));
		_camera = new CameraMove(scene->getCamera(0), _player->getObject());
		_water = new Water(scene->getObject(1), _camera);

		//Игровые объекты
		scene->addObject(ObjDiedTree({ -10,-3,-10 }, 1));
		scene->addObject(ObjTree1({ -15,-3,-15 }, 1));
		scene->addObject(ObjTree2({ -10,-3,-5 }, 1));


		scene->addObject(ObjHouse1({ -10,-2,10 }, 1, { 0,1,0 }, 45));
		scene->addObject(ObjTree1({ -15,-3,15 }, 1));
		scene->addObject(ObjTree2({ -10,-3,5 }, 1));



		scene->addObject(ObjHouse2({ 10,-2,10 }, 1, { 0,1,0 }, 0));
		scene->addObject(ObjDiedTree({ 10,-3,15 }, 1,{0,1,0},43));
		scene->addObject(ObjDiedTree({ 5,-3,15 }, 1.5 ));

		scene->addObject(ObjHouse2({ -15,-2,-10 }, 1, { 0,1,0 }, 58));
		scene->addObject(ObjHouse1({ 15,-2,-15 }, 1 / 1, { 0,1,0 }, 32));


		scene->addObject(ObjTree2({ 15,-3,-20 }, 1.2, { 0,1,0 }, 51));
		scene->addObject(ObjTree1({ 15,-3,15 }, 0.8, { 0,1,0 }, 78));
		scene->addObject(ObjTree2({ 10,-3,5 }, 1.3, { 0,1,0 }, 28));

		scene->addObject(ObjUmbrela({ 3,-4,-23 }, 2, { 0,1,0 }, 28));
		scene->addObject(ObjUmbrela({ 15,-4,-22 }, 2, { 0,1,0 }, 128));
		scene->addObject(ObjUmbrela({ -5,-5,-22.4 }, 2, { 0,1,0 }, 158));
		scene->addObject(ObjUmbrela({ -7,-5,-25 }, 2, { 0,1,0 }, 18));
		//Примитивы
		//scene->addObject(ObjPlane(GL_TRIANGLES, { -2,-1,-2 }, 1, texture, nullptr));
		for (int i = 0; i < 1; i++) {
			scene->addObject(ObjCube(GL_TRIANGLES, { 15, 1*i,-0.7*i }, 2, texture_box, nullptr));
		}
		scene->addObject(ObjSphere(GL_TRIANGLES, { -3,0,3 }, 0.3, texture_ball, nullptr));


		for (int i = 0; i < 3; i++) {
			scene->addObject(ObjCube(GL_TRIANGLES, {-6 + i*3, 1 * i, -20 }, 1, texture_box_beer, nullptr));
		}

		for (int i = 0; i < 5; i++) {
			scene->addObject(ObjCapy(GL_TRIANGLES, { -6 + i * 5, -3, -30 }, {0,1,0}, 60*i));
		}

		for (int i = 0; i < 2; i++) {
			scene->addObject(ObjCapy(GL_TRIANGLES, { 6 + i * 5, -3, -18 }, { 0,1,0 }, -60 * i));
		}



		//Удаляем текстуры из ОЗУ
		delete texture_box;
		delete texture_box_beer;
		delete texture_ball;
	}

	void InitCamera(Engine* engine, Window* window) {
		//Камера
		Camera* mainCamera = engine->getScene(engine->getIdActiveScene())->getCamera(0);
		mainCamera->setPerspective(30, window->getWidth(), window->getHeight(), 0.1, 100.);
		mainCamera->getTransform()->move({ 0,3,10 });
	}
};
