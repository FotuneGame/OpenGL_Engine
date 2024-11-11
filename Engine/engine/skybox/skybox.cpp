#include "Skybox.h"


Skybox::Skybox() {
	_cubeMap = -1;
	_shaderProgramme = -1;
	_model = new Model();
}
Skybox::~Skybox() {
	if (_model)
		delete _model;
}

void Skybox::loadCubeMap(vector <string> texturesPaths) {
	if (texturesPaths.size() != 6) {
		cout << "Ñannot load cube map! Need 6 img " << endl;
		return;
	}

	glGenTextures(1, &_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < texturesPaths.size(); i++) {
		Texture* texture = new Texture(texturesPaths[i].c_str());
		if (texture->getData()) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texture->getChannels() == 4 ? GL_RGBA : GL_RGB, texture->getWitdth(), texture->getHeight(), 0, texture->getChannels() == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture->getData());
		}
		else {
			cout << "Ñannot load cube map ! by path: " << texturesPaths[i] << endl;
		}
		delete texture;
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

int Skybox::render(Camera* camera) {
	if (!_model || !camera)return -2;
	useCubeMap();

	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.f);
	glm::mat4 view = glm::mat4(glm::mat3(camera->getTransform()->getMatrix()));
	int err = _model->render(glm::mat4(1.f), view, camera->getProjection(), camera->getTransform()->getPosition());
	glDepthFunc(GL_LESS);
	camera->render();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return err;
}


Model* Skybox::getModel() {
	return _model;
}



void Skybox::useCubeMap() {
	if (_cubeMap != -1 && _model->getMaterial()->getShaderProgramme()!=-1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);
		glUniform1i(glGetUniformLocation(_model->getMaterial()->getShaderProgramme(), "cubemap"), 0);
	}
}