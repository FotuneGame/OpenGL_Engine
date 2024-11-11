#include "Mesh.h"

Mesh::Mesh() {
	_vao = -1;
	_vertecesLength = 0;
	_indicesLength = 0;
	_normalsLength = 0;
	glGenVertexArrays(1, &_vao);
}

Mesh::~Mesh() {
	
}





void Mesh::loadCoords(glm::vec3* verteces, size_t length)
{
	_vertecesLength = length;

	GLuint coords_vbo = 0;
	glGenBuffers(1, &coords_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, coords_vbo);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(glm::vec3), verteces, GL_STATIC_DRAW);

	glBindVertexArray(_vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
}


void Mesh::loadIndices(GLuint* indices, size_t length)
{
	_indicesLength = length;
	GLuint elementbuffer = 0;
	glBindVertexArray(_vao);
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void Mesh::loadNormals(glm::vec3* normals, size_t length) {
	_normalsLength = length;

	GLuint coords_vbo = 0;
	glGenBuffers(1, &coords_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, coords_vbo);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(glm::vec3), normals, GL_STATIC_DRAW);

	glBindVertexArray(_vao);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
}

void Mesh::loadTextureCoords(glm::vec2* tex, size_t length) {
	_textureCoordsLength = length;

	GLuint tex_vbo = 0;
	glGenBuffers(1, &tex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(glm::vec2), tex, GL_STATIC_DRAW);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);
}



//0 - ok, -4 - file is null, -5 - cannot convert format
int Mesh::loadModelFromFile(const char* filename) {

	std::vector<glm::vec3> outVertices;
	std::vector<glm::vec2> outTextures;
	std::vector<glm::vec3> outNormals;

	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	return loadModelFromFile(filename, outVertices, outTextures, outNormals, temp_vertices, temp_uvs, temp_normals);
}

int Mesh::loadModelFromFile(const char* filename, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals, std::vector< glm::vec3 >& temp_vertices, std::vector< glm::vec2 >& temp_uvs, std::vector< glm::vec3 >& temp_normals) {
	if (!filename) return -4;

	int status = 0;
	string str = filename;
	string extension = str.substr(str.find_last_of(".") + 1);


	if (extension == "obj") {
		status = loadModelFromOBJ(filename, outVertices, outTextures, outNormals, temp_vertices, temp_uvs, temp_normals);
		if (status) return status;
		if (outVertices.size())
			loadCoords(&outVertices[0], outVertices.size());
		if(outNormals.size())
			loadNormals(&outNormals[0], outNormals.size());
		if (outTextures.size())
			loadTextureCoords(&outTextures[0], outTextures.size());
	}
	//ADD new format if you need

	return -5;
}


const GLuint Mesh::getVAO() {
	return _vao;
}
const size_t Mesh::getVAOLength() {
	return _vertecesLength;
}

const size_t Mesh::getIndicesLength() {
	return _indicesLength;
}

const size_t Mesh::getNormalsLength() {
	return _normalsLength;
}
const size_t Mesh::getTextureCoordsLength() {
	return _textureCoordsLength;
}