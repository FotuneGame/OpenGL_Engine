#include "Shadow.h"

Shadow::Shadow(const char* vectorShader, const char* fragmentShader) {
	_width = 4096;
	_height = 4096;
	_shaderProgramme = -1;
	_distanceDir = 10.f;
	loadShaders(vectorShader, fragmentShader);
	init();
}
Shadow::Shadow(const char* vectorShader, const char* fragmentShader, int width, int heigth, float distanceDir) {
	_width = width;
	_height = heigth;
	_shaderProgramme = -1;
	_distanceDir = distanceDir;
	loadShaders(vectorShader, fragmentShader);
	init();
}
Shadow::~Shadow() {

}

//Создаём фреймбуфер только для глубины и связываем его с текстурой
void Shadow::init() {
	_fbo = -1;
	_shadowMap = -1;
	glGenFramebuffers(1, &_fbo);

	glGenTextures(1, &_shadowMap);
	glBindTexture(GL_TEXTURE_2D, _shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		cout << "(Shadow map) FB error, status: " << status <<endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int Shadow::render(unordered_map<int, Object*> objects, unordered_map<int, Light*> lights, Camera* camera) {
	if (_fbo == -1 || _shadowMap == -1 || _shaderProgramme ==-1) return -3;

	
	Light* dirLight = nullptr;
	unordered_map<int, Light*>::iterator itLights;
	for (itLights = lights.begin(); itLights != lights.end(); itLights++) {
		if (itLights->second->getMode() == DIRACTION_LIGHT_MODE)
			dirLight = itLights->second;
		else {
			if(itLights->second->getShadowPS())
				renderPS(objects,itLights->second,camera);
		}
	}
	
	if(dirLight)
		renderDir(objects, dirLight, camera);
	return 0;
}


void Shadow::renderPS(unordered_map<int, Object*> objects, Light* light, Camera* camera) {	

	//Свет дальше отрисовки
	if (glm::length(light->pointLight.position - camera->getTransform()->getPosition()) > light->getShadowPS()->getDistance()) return;

	glm::vec3 lightPos = { 0,0,0 };
	if (light->getMode() == POINT_LIGHT_MODE)
		lightPos = light->pointLight.position;
	else if (light->getMode() == SPOT_LIGHT_MODE)
		lightPos = light->spotLight.position;

	//Связываем шейдерную программу света
	int err = light->getShadowPS()->bind(lightPos);
	if (err) return;

	//Отрисовка сцены
	unordered_map<int, Object*>::iterator itObject;
	for (itObject = objects.begin(); itObject != objects.end(); itObject++) {

		glBindVertexArray(itObject->second->getModel()->getMesh()->getVAO());

		//vs
		GLuint TransformShaderVarM = glGetUniformLocation(light->getShadowPS()->getShaderPrograme(), "M");
		glUniformMatrix4fv(TransformShaderVarM, 1, GL_FALSE, &(itObject->second->getTransform()->getMatrix())[0][0]);

		if (itObject->second->getModel()->getMesh()->getIndicesLength() > 0)
			glDrawElements(GL_TRIANGLES, itObject->second->getModel()->getMesh()->getIndicesLength(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, itObject->second->getModel()->getMesh()->getVAOLength());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Shadow::renderDir(unordered_map<int, Object*> objects, Light* dirLight, Camera* camera) {
	if (_fbo == -1 || _shadowMap == -1 || _shaderProgramme == -1) return;

	glViewport(0, 0, _width, _height);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (!dirLight) return;
	glm::mat4 lightProjection = glm::ortho(-7.f * _distanceDir, 7.f * _distanceDir, -7.f * _distanceDir, 7.f * _distanceDir, 0.01f, 10.f * _distanceDir);
	glm::mat4 lightView = glm::lookAt(camera->getTransform()->getPosition() - dirLight->dirLight.direction * (_distanceDir), camera->getTransform()->getTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
	dirLight->dirLight.projection = lightProjection;
	dirLight->dirLight.view = lightView;
	//используем шейдерную программу
	glUseProgram(_shaderProgramme);
	//биндим данные в шейдер
	GLuint transformShaderVarLV = glGetUniformLocation(_shaderProgramme, "LV");
	glUniformMatrix4fv(transformShaderVarLV, 1, GL_FALSE, &(lightView)[0][0]);
	GLuint transformShaderVarLP = glGetUniformLocation(_shaderProgramme, "LP");
	glUniformMatrix4fv(transformShaderVarLP, 1, GL_FALSE, &(lightProjection)[0][0]);
	//рендер сцены без освещения c шейдером освещения
	unordered_map<int, Object*>::iterator itObject;
	for (itObject = objects.begin(); itObject != objects.end(); itObject++) {
		//Используем вершины объекта
		glBindVertexArray(itObject->second->getModel()->getMesh()->getVAO());
		//Биндим в шейдер
		GLuint transformShaderVarM = glGetUniformLocation(_shaderProgramme, "M");
		glUniformMatrix4fv(transformShaderVarM, 1, GL_FALSE, &(itObject->second->getTransform()->getMatrix())[0][0]);

		if (itObject->second->getModel()->getMesh()->getIndicesLength() > 0)
			glDrawElements(GL_TRIANGLES, itObject->second->getModel()->getMesh()->getIndicesLength(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, itObject->second->getModel()->getMesh()->getVAOLength());
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




GLuint Shadow::getShadowMap() {
	return _shadowMap;
}



void Shadow::loadShaders(const char* vectorShader, const char* fragmentShader) {
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
	glLinkProgram(_shaderProgramme);//Компоновка шейдерной программы
}
string Shadow::readShaderFile(const char* filename) {
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