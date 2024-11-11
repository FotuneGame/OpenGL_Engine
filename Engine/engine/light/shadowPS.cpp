#include "ShadowPS.h"

ShadowPS::ShadowPS(const char* vectorShader, const char* geometricShader, const char* fragmentShader) {
	_width = 512;
	_height = 512;
	_shaderProgramme = -1;
	_shadowProjection = glm::mat4(1.f);
	_distance = 25.f;
	loadShaders(vectorShader, geometricShader, fragmentShader);
	init();
}
ShadowPS::ShadowPS(const char* vectorShader, const char* geometricShader, const char* fragmentShader, int width, int heigth, float distance) {
	_width = width;
	_height = heigth;
	_shaderProgramme = -1;
	_shadowProjection = glm::mat4(1.f);
	_distance = distance;
	loadShaders(vectorShader, geometricShader, fragmentShader);
	init();
}
ShadowPS::~ShadowPS() {

}

//Создаёт кубическую карты и связывает её с фреймбуфером
void ShadowPS::init() {
	_fbo = -1;
	_shadowCubeMap = -1;
	glGenFramebuffers(1, &_fbo);

	glGenTextures(1, &_shadowCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _shadowCubeMap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,_width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _shadowCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		cout << "(ShadowPS CubeMap) FB error, status: " << status << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int ShadowPS::bind(glm::vec3 lightPos) {
	if (_fbo == -1 || _shadowCubeMap == -1 || _shaderProgramme == -1 || _distance <=0) return -4;

	glViewport(0, 0, _width, _height);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClear( GL_DEPTH_BUFFER_BIT);

	//Настройка матриц для положения мира относительно источника света
	_shadowProjection = glm::perspective(glm::radians(90.0f), (float)_width / _height, 0.01f, _distance);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(_shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(_shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(_shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(_shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(_shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(_shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	//используем шейдерную программу
	glUseProgram(_shaderProgramme);

	//Биндим в шейдер
	//geom
	for (int i = 0; i < 6; ++i) {
		string selector = "shadowMatrices[" + std::to_string(i) + "]";
		GLuint transformShaderVarShadowMatrices = glGetUniformLocation(_shaderProgramme, selector.c_str());
		glUniformMatrix4fv(transformShaderVarShadowMatrices, 1, GL_FALSE, &(shadowTransforms[i])[0][0]);
	}
	//fs
	GLuint ShaderVarLightPos = glGetUniformLocation(_shaderProgramme, "light_pos");
	glUniform3fv(ShaderVarLightPos, 1, &(lightPos)[0]);
	GLuint ShaderVarFarPlane = glGetUniformLocation(_shaderProgramme, "far_plane");
	glUniform1f(ShaderVarFarPlane, _distance);



	return 0;
}

GLuint ShadowPS::getShadowCubeMap() {
	return _shadowCubeMap;
}
glm::mat4 ShadowPS::getShadowProjection() {
	return _shadowProjection;
}
float ShadowPS::getDistance() {
	return _distance;
}



///Подправить ++++++++++++++++==
void ShadowPS::loadShaders(const char* vectorShader, const char* geometricShader, const char* fragmentShader) {
	GLint result = GL_FALSE;
	int infoLogLength;
	_shaderProgramme = glCreateProgram();

	//Считывание шедеров с файлов
	string vstext = readShaderFile(vectorShader);
	const char* vertex_shader = vstext.c_str();
	string geomtext = readShaderFile(geometricShader);
	const char* geom_shader = geomtext.c_str();
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


	GLuint geom = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geom, 1, &geom_shader, NULL);
	glCompileShader(geom);

	//проверка на ошибку компиляции шейдера geom
	glGetShaderiv(geom, GL_COMPILE_STATUS, &result);
	glGetShaderiv(geom, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(geom, infoLogLength, NULL, errorMessage);
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
	glAttachShader(_shaderProgramme, geom);
	glAttachShader(_shaderProgramme, fs);
	glBindAttribLocation(_shaderProgramme, 0, "vertex_position");
	glLinkProgram(_shaderProgramme);//Компоновка шейдерной программы
}


string ShadowPS::readShaderFile(const char* filename) {
	string res;
	ifstream file(filename, ios::in);
	if (file.is_open())
	{
		std::stringstream sstr; // Буфер для чтения
		sstr << file.rdbuf(); // Считываем файл
		res = sstr.str(); //Получаем строку из буфера
		file.close(); //Закрываем файл
	}
	return res;
}


GLuint ShadowPS::getShaderPrograme() {
	return _shaderProgramme;
}