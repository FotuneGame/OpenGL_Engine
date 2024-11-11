#include "Material.h"


Material::Material(Mesh* mesh) {
	this->_mesh = mesh;
	_shaderProgramme = -1;
	_transformTextures = new Transform();
	MaterialSettings defaultSettings;
	defaultSettings.specular = { 0.5f, 0.5f, 0.5f };
	defaultSettings.shininess = 5.f;
	_textures[0] = -1;
	_textures[1] = -1;
	_textures[2] = -1;
	loadMaterialSettings(defaultSettings);
	clearLightsCounter();
}
Material::~Material() {
	if (_transformTextures)
		delete _transformTextures;
}

void Material::setMesh(Mesh* mesh) {
	this->_mesh = mesh;
}

void Material::loadLight(Light* light) {
	if (_shaderProgramme < 0) return;

	glUseProgram(_shaderProgramme);
	int mode = light->getMode();
	
	if (mode == DIRACTION_LIGHT_MODE) {
		GLuint d_direction = glGetUniformLocation(_shaderProgramme, "lights.dirLight.direction");
		glUniform3fv(d_direction, 1, &(light->dirLight.direction)[0]);
		GLuint d_ambient = glGetUniformLocation(_shaderProgramme, "lights.dirLight.ambient");
		glUniform3fv(d_ambient, 1, &(light->dirLight.ambient)[0]);
		GLuint d_diffuse = glGetUniformLocation(_shaderProgramme, "lights.dirLight.diffuse");
		glUniform3fv(d_diffuse, 1, &(light->dirLight.diffuse)[0]);
		GLuint d_specular = glGetUniformLocation(_shaderProgramme, "lights.dirLight.specular");
		glUniform3fv(d_specular, 1, &(light->dirLight.specular)[0]);

		GLuint transformShaderVarLV = glGetUniformLocation(_shaderProgramme, "LV_DIR");
		glUniformMatrix4fv(transformShaderVarLV, 1, GL_FALSE, &(light->dirLight.view)[0][0]);
		GLuint transformShaderVarLP = glGetUniformLocation(_shaderProgramme, "LP_DIR");
		glUniformMatrix4fv(transformShaderVarLP, 1, GL_FALSE, &(light->dirLight.projection)[0][0]);

		_dirLength++;
	}
	else if (mode == POINT_LIGHT_MODE) {
		GLuint lnp = glGetUniformLocation(_shaderProgramme, "lights.numPLights");
		glUniform1i(lnp, _pointLength + 1);

		string selector = "lights.pointLights[" + std::to_string(_pointLength) + "]";

		GLuint p_position = glGetUniformLocation(_shaderProgramme, (selector +".position").c_str());
		glUniform3fv(p_position, 1, &(light->pointLight.position)[0]);
		GLuint p_constant = glGetUniformLocation(_shaderProgramme, (selector + ".constant").c_str());
		glUniform1f(p_constant, light->pointLight.constant);
		GLuint p_linear = glGetUniformLocation(_shaderProgramme, (selector + ".linear").c_str());
		glUniform1f(p_linear, light->pointLight.linear);
		GLuint p_quadratic = glGetUniformLocation(_shaderProgramme, (selector + ".quadratic").c_str());
		glUniform1f(p_quadratic, light->pointLight.quadratic);

		GLuint p_ambient = glGetUniformLocation(_shaderProgramme, (selector + ".ambient").c_str());
		glUniform3fv(p_ambient, 1, &(light->pointLight.ambient)[0]);
		GLuint p_diffuse = glGetUniformLocation(_shaderProgramme, (selector + ".diffuse").c_str());
		glUniform3fv(p_diffuse, 1, &(light->pointLight.diffuse)[0]);
		GLuint p_specular = glGetUniformLocation(_shaderProgramme, (selector + ".specular").c_str());
		glUniform3fv(p_specular, 1, &(light->pointLight.specular)[0]);

		//Добавление теней
		if (light->getShadowPS()) {
			GLuint  p_proj = glGetUniformLocation(_shaderProgramme, (selector + ".projection").c_str());
			glUniformMatrix4fv(p_proj, 1, GL_FALSE, &(light->getShadowPS()->getShadowProjection())[0][0]);
			GLuint  p_farPlane = glGetUniformLocation(_shaderProgramme, (selector + ".farPlane").c_str());
			glUniform1f(p_farPlane, light->getShadowPS()->getDistance());
			GLuint  p_idTexture = glGetUniformLocation(_shaderProgramme, (selector + ".idShadowCubeMap").c_str());
			glUniform1i(p_idTexture, _textureId - 3);

			glActiveTexture(GL_TEXTURE0 + _textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, light->getShadowPS()->getShadowCubeMap());
			string selector_cubeMap = "shadowCubeMaps[" + std::to_string(_textureId - 3) + "]";
			glUniform1i(glGetUniformLocation(_shaderProgramme, (selector_cubeMap).c_str()), _textureId);
			_textureId++;
		}
		_pointLength++;
	}
	else if (mode == SPOT_LIGHT_MODE) {
		GLuint lsp = glGetUniformLocation(_shaderProgramme, "lights.numSLights");
		glUniform1i(lsp, _spotLength + 1);

		string selector = "lights.spotLights[" + std::to_string(_spotLength)+"]";

		GLuint s_position = glGetUniformLocation(_shaderProgramme, (selector + ".position").c_str());
		glUniform3fv(s_position, 1, &(light->spotLight.position)[0]);
		GLuint s_direction = glGetUniformLocation(_shaderProgramme, (selector + ".direction").c_str());
		glUniform3fv(s_direction, 1, &(light->spotLight.direction)[0]);

		GLuint s_cutOff = glGetUniformLocation(_shaderProgramme, (selector + ".cutOff").c_str());
		glUniform1f(s_cutOff, light->spotLight.cutOff);
		GLuint s_outerCutOff = glGetUniformLocation(_shaderProgramme, (selector + ".outerCutOff").c_str());
		glUniform1f(s_outerCutOff, light->spotLight.outerCutOff);
		GLuint s_constant = glGetUniformLocation(_shaderProgramme, (selector + ".constant").c_str());
		glUniform1f(s_constant, light->spotLight.constant);
		GLuint s_linear = glGetUniformLocation(_shaderProgramme, (selector + ".linear").c_str());
		glUniform1f(s_linear, light->spotLight.linear);
		GLuint s_quadratic = glGetUniformLocation(_shaderProgramme, (selector + ".quadratic").c_str());
		glUniform1f(s_quadratic, light->spotLight.quadratic);

		GLuint s_ambient = glGetUniformLocation(_shaderProgramme, (selector + ".ambient").c_str());
		glUniform3fv(s_ambient, 1, &(light->spotLight.ambient)[0]);
		GLuint s_diffuse = glGetUniformLocation(_shaderProgramme, (selector + ".diffuse").c_str());
		glUniform3fv(s_diffuse, 1, &(light->spotLight.diffuse)[0]);
		GLuint s_specular = glGetUniformLocation(_shaderProgramme, (selector + ".specular").c_str());
		glUniform3fv(s_specular, 1, &(light->spotLight.specular)[0]);


		//Добавление теней
		if (light->getShadowPS()) {
			GLuint  p_proj = glGetUniformLocation(_shaderProgramme, (selector + ".projection").c_str());
			glUniformMatrix4fv(p_proj, 1, GL_FALSE, &(light->getShadowPS()->getShadowProjection())[0][0]);
			GLuint  p_farPlane = glGetUniformLocation(_shaderProgramme, (selector + ".farPlane").c_str());
			glUniform1f(p_farPlane, light->getShadowPS()->getDistance());
			GLuint  p_idTexture = glGetUniformLocation(_shaderProgramme, (selector + ".idShadowCubeMap").c_str());
			glUniform1i(p_idTexture, _textureId-3);

			glActiveTexture(GL_TEXTURE0 + _textureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, light->getShadowPS()->getShadowCubeMap());
			string selector_cubeMap = "shadowCubeMaps[" + std::to_string(_textureId - 3) + "]";
			glUniform1i(glGetUniformLocation(_shaderProgramme, (selector_cubeMap).c_str()), _textureId);
			_textureId++;
		}
		_spotLength++;
	}
}

void Material::loadShadowMap(GLuint _shadowMap) {
	_textures[2] = _shadowMap;
}





void Material::useMaterial() {
	GLuint MaterialShaderVarSpec = glGetUniformLocation(_shaderProgramme, "material.specular");
	glUniform3fv(MaterialShaderVarSpec, 1, &_materialSettings.specular[0]);
	GLuint MaterialShaderVarShininess = glGetUniformLocation(_shaderProgramme, "material.shininess");
	glUniform1f(MaterialShaderVarShininess, _materialSettings.shininess);
	useTexture();
}

void Material::useTexture() {
	if (_textures[0] != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textures[0]);
		glUniform1i(glGetUniformLocation(_shaderProgramme, "material.texture"), 0);
	}
	if (_textures[1] != -1) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _textures[1]);
		glUniform1i(glGetUniformLocation(_shaderProgramme, "material.specularmap"), 1);
	}
	if (_textures[2] != -1) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _textures[2]);
		glUniform1i(glGetUniformLocation(_shaderProgramme, "shadowmap"), 2);
	}

	GLuint transformShaderVarTM = glGetUniformLocation(_shaderProgramme, "TM");
	glUniformMatrix4fv(transformShaderVarTM, 1, GL_FALSE, &(_transformTextures->getMatrix()[0][0]));
}

void Material::loadMaterialSettings(MaterialSettings& materialSettings) {
	this->_materialSettings.shininess = materialSettings.shininess;
	this->_materialSettings.specular = materialSettings.specular;
}

void Material::loadShaders(const char* vectorShader, const char* fragmentShader)
{
	GLint result = GL_FALSE;
	int infoLogLength;
	_shaderProgramme = glCreateProgram();

	//Считывание шедеров с файлов
	string vstext = readShaderFile(vectorShader);
	const char* vertex_shader = vstext.c_str();
	string fstext = readShaderFile(fragmentShader);
	const char* fragment_shader = fstext.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	//проверка на ошибку компиляции шейдера vs
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(vs, infoLogLength, NULL, errorMessage);
		std::cout << errorMessage;
		delete[] errorMessage;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	//проверка на ошибку компиляции шейдера fs
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(fs, infoLogLength, NULL, errorMessage);
		std::cout << errorMessage;
		delete[] errorMessage;
	}

	//Сборка программы
	glAttachShader(_shaderProgramme, vs);
	glAttachShader(_shaderProgramme, fs);
	glBindAttribLocation(_shaderProgramme, 0, "vertex_position");
	glBindAttribLocation(_shaderProgramme, 1, "vertex_color");
	glBindAttribLocation(_shaderProgramme, 2, "vertex_normal");
	glBindAttribLocation(_shaderProgramme, 3, "vertex_texture");
	glLinkProgram(_shaderProgramme);//Компоновка шейдерной программы
	clearLightsCounter(); //отчистка буффера кол-ва компонентов света на сцене
}


void Material::loadColor(glm::vec3 color) {
	glm::vec3* colors = new glm::vec3[_mesh->getVAOLength()];
	for (int i = 0; i < _mesh->getVAOLength(); i++) {
		colors[i] = color;
	}
	loadColors(colors, _mesh->getVAOLength());
	delete[] colors;
}
void Material::loadColors(glm::vec3* colors, size_t length) {
	if (!colors || !length || !_mesh->getVAO()) return;
	GLuint colors_vbo = 0;
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(glm::vec3), colors, GL_STATIC_DRAW);

	glBindVertexArray(_mesh->getVAO());
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
}




void Material::loadTextures(Texture* texture, Texture* specularMap) {
	glGenTextures(2, _textures);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, _textures[0]);
		settingsTexture(texture);
	}
	else {
		_textures[0] = -1;
	}
	if (specularMap) {
		glBindTexture(GL_TEXTURE_2D, _textures[1]);
		settingsTexture(specularMap);
	}
	else {
		_textures[1] = -1;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}



void Material::settingsTexture(Texture* texture) {
	glTexImage2D(GL_TEXTURE_2D, 0, texture->getChannels() == 4 ? GL_RGBA : GL_RGB, texture->getWitdth(), texture->getHeight(), 0, texture->getChannels() == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture->getData());

	//минимап
	if (!texture->minimap) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filter);
	}
	else {
		if (texture->filter == GL_LINEAR)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filter);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Мазаичный тайловый режим 
	if (texture->repaet > 0) {
		if (texture->repaet == GL_CLAMP_TO_BORDER) {
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &texture->borderColorClampMode[0]);
		}
		else {
			if (texture->horizontalRepeat)
				glTexParameteri(GL_TEXTURE_2D, texture->repaet, GL_TEXTURE_WRAP_T);
			else
				glTexParameteri(GL_TEXTURE_2D, texture->repaet, GL_TEXTURE_WRAP_S);
		}
	}
}



const GLuint Material::getShaderProgramme() {
	return _shaderProgramme;
}

void Material::clearLightsCounter() {
	_textureId = 3;
	_dirLength = 0;
	_pointLength = 0;
	_spotLength = 0;
}

Transform* Material::getTransformTextures() {
	return _transformTextures;
}