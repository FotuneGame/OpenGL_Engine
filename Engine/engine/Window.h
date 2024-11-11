#pragma once
#include "Engine.h"
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
#define KEYS_LENGTH 1024


class Window {
public:
	Window();
	Window(int width, int height, bool isFullscreen, const char* name);
	~Window();

	int setKeyCallback(Engine* engine,void (*key_callback)(Engine* engine, Window* window, int keys[KEYS_LENGTH], int _keyMods));
	int render(Engine* engine,int (*update)(Engine* engine, Window* window));
	int getWidth();
	int getHeight();
	double getDeltaTime();
	double getLastFrame();
	void close();
private:

	GLFWwindow* _window;
	Engine* _engine;

	int _width;
	int _height;
	bool _isFullscreen;
	const char* _name;

	double _deltaTime;
	double _lastFrame;

	static void staticWindowSizeCallback(GLFWwindow* window, int width, int height);
	void windowSizeCallback(GLFWwindow* window, int width, int height);

	int _keys[KEYS_LENGTH];
	int _keyMods;
	void (*_key_callback)(Engine* engine, Window* window, int keys[KEYS_LENGTH], int _keyMods);
	static void staticWindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void initWindow();
};