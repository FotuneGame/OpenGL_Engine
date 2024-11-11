#include "engine/Window.h"
#include "SceneGame.h"
#include <iostream>
#include <math.h>



int update(Engine* engine, Window* window);
void key_callback(Engine* engine, Window* window, int keys[KEYS_LENGTH], int _keyMods);



SceneGame* game;
int main() {
	Window* window = new Window(680, 680, false, "Engine V8");
	Engine* engine = new Engine();

	//Создаём игровую сцену
	Scene* scene = new Scene("Game");
	engine->addScene(scene);
	game = new SceneGame(engine, engine->getScene(0), window);

	//Реакция на клавиатуру
	window->setKeyCallback(engine, key_callback);


	//Рендер
	int status = window->render(engine, update);

	//Чистим игру
	delete game;
	//Чистим движок
	delete engine;
	delete window;
	return status;
}


int update(Engine* engine, Window* window) {
	switch (engine->getIdActiveScene()) {
	case 0:
		if (game)
			game->update(engine->getScene(engine->getIdActiveScene()), window);
		break;
	}
	return 0;
}


void key_callback(Engine* engine, Window* window, int keys[KEYS_LENGTH], int _keyMods)
{
	switch (engine->getIdActiveScene()) {
	case 0:
		if (game)
			game->key_callback(engine->getScene(engine->getIdActiveScene()), window, keys, _keyMods);
		break;
	}
}

