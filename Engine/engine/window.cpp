#include "Window.h"

Window::Window() {
	_width = 640;
	_height = 480;
	_isFullscreen = false;
	_name = "Test";
	initWindow();
}



Window::Window(int width, int height, bool isFullscreen,const char* name) {
	this->_width = width;
	this->_height = width;
	this->_isFullscreen = isFullscreen;
	this->_name = name;
	initWindow();
}


void Window::initWindow() {
	_deltaTime = 0;
	_lastFrame = 0;
	_window = nullptr;
	_key_callback = nullptr;
	_engine = nullptr;
	//Подключение GLFW
	if (!glfwInit())
	{
		std::cerr << "ERROR: could not start GLFW3\n";
		std::exit(-1);
	}
	// Это для MacOS
	/* 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); 
	*/

	if (_isFullscreen)
	{
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		const GLFWvidmode* vmode = glfwGetVideoMode(mon);
		_width = vmode->width;
		_height = vmode->height;
		_window = glfwCreateWindow(_width, _height, _name, mon, NULL);
	}
	else
		_window = glfwCreateWindow(_width, _height, _name, NULL, NULL);

	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, &staticWindowSizeCallback);
	glfwSetKeyCallback(_window, &staticWindowKeyCallback);
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);
}


Window::~Window() {
	// Просто выключение GLFW
	// Сюда же можно добавить очистку памяти, если будет нужно
	if(_window)
		glfwDestroyWindow(_window);
	//glfwTerminate();
}

int Window::render(Engine* engine,int (*update)(Engine* engine,Window* window)) {
	int code =0;
	double cuurentFrame;
	while (!code && _window && _engine && !glfwWindowShouldClose(_window)) {
		if (update)
			code = update(engine, this);
		code = engine->render(_width,_height, _deltaTime);
		if (code) return code;
		glfwPollEvents();
		glfwSwapBuffers(_window);

		_key_callback(_engine, this, _keys, _keyMods);

		cuurentFrame = glfwGetTime();
		_deltaTime = cuurentFrame - _lastFrame;
		_lastFrame = cuurentFrame;
	}
	return 0;
}

void Window::close() {
	glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

int Window::getWidth() {
	return _width;
}
int Window::getHeight() {
	return _height;
}
double Window::getDeltaTime() {
	return _deltaTime;
}
double Window::getLastFrame() {
	return _lastFrame;
}


void Window::windowSizeCallback(GLFWwindow* window, int width, int height) {
	this->_width = width;
	this->_height = height;
	glViewport(0, 0, _width, _height);
}

void Window::staticWindowSizeCallback(GLFWwindow* window, int width, int height) {
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->windowSizeCallback(window, width,height);
}




int Window::setKeyCallback(Engine* engine, void (*key_callback)(Engine* engine, Window* window, int keys[KEYS_LENGTH],int _keyMods)) {
	if (!key_callback || !engine) return 1;
	_key_callback = key_callback;
	_engine = engine;
	return 0;
}

void Window::staticWindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
	actualWindow->windowKeyCallback(window,key,scancode,action,mods);
}

void Window::windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		_keys[key] = action;
	else if (action == GLFW_RELEASE)
		_keys[key] = 0;
	_keyMods = mods;
}


