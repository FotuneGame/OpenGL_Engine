#include "Model.h"


Model::Model() {
	_mesh = new Mesh();
	_material = new Material(_mesh);
	init();
};
Model::Model(Mesh* mesh) {
	this->_mesh = mesh;
	_material = new Material(_mesh);
	init();
};
Model::~Model() {
	if(_mesh)
		delete _mesh;
	if (_material)
		delete _material;
};
void Model::init() {
	sizeBrush = 10;
	_mode = GL_TRIANGLES;
	_polygonRenderData[0]._mode = GL_FILL;
	_polygonRenderData[0]._type = GL_FRONT;
	_polygonRenderData[1]._mode = GL_FILL;
	_polygonRenderData[1]._type = GL_BACK;
}

int Model::render(const glm::mat4 transformMatrix, const glm::mat4 transformCamera, const glm::mat4 projectionCamera, const glm::vec3 campos) {
	if (!(_material->getShaderProgramme() > 0 && _mesh->getVAO() > 0 && _mesh->getVAOLength() > 0) || !_material)return -3;

	glUseProgram(_material->getShaderProgramme());
	glBindVertexArray(_mesh->getVAO());


	GLuint transformShaderVarP = glGetUniformLocation(_material->getShaderProgramme(), "P");	glUniformMatrix4fv(transformShaderVarP, 1, GL_FALSE, &projectionCamera[0][0]);	GLuint transformShaderVarV = glGetUniformLocation(_material->getShaderProgramme(), "V");	glUniformMatrix4fv(transformShaderVarV, 1, GL_FALSE, &transformCamera[0][0]);	GLuint transformShaderVarM = glGetUniformLocation(_material->getShaderProgramme(), "M");	glUniformMatrix4fv(transformShaderVarM, 1, GL_FALSE, &transformMatrix[0][0]);	GLuint transformShaderVarCampos = glGetUniformLocation(_material->getShaderProgramme(), "campos");	glUniform3fv(transformShaderVarCampos, 1, &campos[0]);	
	_material->useMaterial();

	settingModeRender();

	if (_mesh->getIndicesLength() > 0)
		glDrawElements(_mode, _mesh->getIndicesLength(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(_mode, 0, _mesh->getVAOLength());


	_material->clearLightsCounter(); //отчистка буффера кол-ва компонентов света на сцене

	return 0;
}




void Model::setMode(GLuint mode) {
	if (mode >= 0) {
		_mode = mode;
	}
}



void Model::settingModeRender() {
	switch (_mode) {
	case GL_POINTS:
		glEnable(GL_POINT_SMOOTH); // округление не работает
		break;
	case GL_LINES:
		glEnable(GL_LINE_SMOOTH);
		break;
	case GL_LINE_STRIP:
		glEnable(GL_LINE_SMOOTH);
		break;
	case GL_LINE_LOOP:
		glEnable(GL_LINE_SMOOTH);
		break;
	default:
		glPolygonMode(_polygonRenderData[0]._type, _polygonRenderData[0]._mode);
		glPolygonMode(_polygonRenderData[1]._type, _polygonRenderData[1]._mode);
		break;
	}
	glPointSize(sizeBrush);
	glLineWidth(sizeBrush);
}


void Model::setPolygonMode(GLuint type, GLuint mode) {
	switch (type)
	{
		case GL_FRONT_AND_BACK:
			_polygonRenderData[0]._type = type;
			_polygonRenderData[0]._mode = mode;
			_polygonRenderData[1]._type = type;
			_polygonRenderData[1]._mode = mode;
			break;
		case GL_BACK:
			_polygonRenderData[1]._type = type;
			_polygonRenderData[1]._mode = mode;
		default:
			_polygonRenderData[0]._type = type;
			_polygonRenderData[0]._mode = mode;
			break;
	}
}

Material* Model::getMaterial() {
	return _material;
}
Mesh* Model::getMesh() {
	return _mesh;
}