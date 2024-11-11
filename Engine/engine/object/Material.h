#pragma once
#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include "Mesh.h"
#include "../light/Light.h"
#include "../component/Texture.h"

struct MaterialSettings {
	glm::vec3 specular;
	float shininess;
};

class Material {
public:
	Material(Mesh* mesh);
	~Material();

	void setMesh(Mesh* mesh);
	void loadColor(glm::vec3 color);
	void loadColors(glm::vec3* colors, size_t length);
	void loadShaders(const char* vectorShader, const char* fragmentShader);
	void loadMaterialSettings(MaterialSettings& materialSettings);
	void loadLight(Light* light);
	void loadTextures(Texture* texture, Texture* specularMap);
	void loadShadowMap(GLuint _shadowMap);

	void clearLightsCounter();
	void useMaterial();
	void useTexture();
	const GLuint getShaderProgramme();

	Transform* getTransformTextures();

private:
	size_t _textureId;
	size_t _dirLength;
	size_t _pointLength;
	size_t _spotLength;

	MaterialSettings _materialSettings;
	Mesh* _mesh;
	GLuint _shaderProgramme;
	
	//texture  - 0, specularmap - 1, shadowmap - 2
	GLuint _textures[3];

	void settingsTexture(Texture* texture);
	static string readShaderFile(const char* filename);

	Transform* _transformTextures;
};