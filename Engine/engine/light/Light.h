#pragma once
#include <iostream>
using namespace std;
#include "ShadowPS.h"
#include "../component/Transform.h"

enum ModeLight {
	DIRACTION_LIGHT_MODE,
	POINT_LIGHT_MODE,
	SPOT_LIGHT_MODE
};

class Light {
public:
	Light();
	Light(const char* name);
	~Light();

	void setMode(ModeLight mode);
	const int getMode();
	const char* getName();

	struct DirLight {
		glm::mat4 projection;
		glm::mat4 view;
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} dirLight;

	struct PointLight
	{
		glm::vec3 position;
		// Параметры затухания
		float constant;
		float linear;
		float quadratic;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} pointLight;

	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 direction;
		// Параметры угла в вид cos
		float cutOff;
		float outerCutOff;
		// Параметры затухания
		float constant;
		float linear;
		float quadratic;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} spotLight;

	void setShadowPS(ShadowPS* shadow);
	ShadowPS* getShadowPS();

private:
	int _mode;
	const char* _name;
	ShadowPS* _shadow;

	void init();
};
