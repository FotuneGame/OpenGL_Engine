#pragma once
#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../light/Light.h"

class Mesh {
public:
	Mesh();
	~Mesh();

	void loadCoords(glm::vec3* verteces, size_t length);
	void loadIndices(GLuint* indices, size_t length);
	void loadNormals(glm::vec3* normals, size_t length);
	void loadTextureCoords(glm::vec2* tex, size_t length);
	int loadModelFromFile(const char* filename);
	int loadModelFromFile(const char* filename, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals, std::vector< glm::vec3 >& temp_vertices, std::vector< glm::vec2 >& temp_uvs, std::vector< glm::vec3 >& temp_normals);
	const GLuint getVAO();
	const size_t getVAOLength();
	const size_t getIndicesLength();
	const size_t getNormalsLength();
	const size_t getTextureCoordsLength();

protected:
	GLuint _vao;
	size_t _vertecesLength;
	size_t _indicesLength;
	size_t _normalsLength;
	size_t _textureCoordsLength;

	static int loadModelFromOBJ(const char* filename, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals, std::vector< glm::vec3 >& temp_vertices, std::vector< glm::vec2 >& temp_uvs, std::vector< glm::vec3 >& temp_normals);
};