#include "Mesh.h"

//-6 - cannot open file, 0 -ok
int Mesh::loadModelFromOBJ(const char* filename, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals, std::vector< glm::vec3 >& temp_vertices, std::vector< glm::vec2 >& temp_uvs, std::vector< glm::vec3 >& temp_normals) {

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	bool isChecked = false;

	string buf;
	std::stringstream sstr;

	ifstream file(filename, ios::in);
	if (file.is_open())
	{
		sstr << file.rdbuf();
		sstr << " \nend\0";
		file.close();
	}
	else
		return -6;

	outNormals.clear();
	outVertices.clear();
	outTextures.clear();

	while (isChecked || sstr >> buf) {
		isChecked = false;
		if (buf == "v"){
			glm::vec3 vertex;
			sstr >> vertex.x;
			sstr >> vertex.y;
			sstr >> vertex.z;
			temp_vertices.push_back(vertex);
		}else if (buf == "vt"){
			glm::vec2 texture;
			sstr >> texture.x;
			sstr >> texture.y;
			temp_uvs.push_back(texture);
		}else if (buf == "vn"){
			glm::vec3 normal;
			sstr >> normal.x;
			sstr >> normal.y;
			sstr >> normal.z;
			temp_normals.push_back(normal);
		}else if (buf == "f"){
			vector<string> vertexes;
			vector<int> vertindexes;

			while (!isChecked)
			{
				sstr >> buf;
				if (buf == "end") break;
				if (buf.find('/') != string::npos)
					vertexes.push_back(buf);
				else{
					for (string vert : vertexes)
					{
						std::replace(vert.begin(), vert.end(), '/', ' ');
						std::stringstream tmpstream(vert);
						int v, vt, n;
						tmpstream >> v;
						tmpstream >> vt;
						tmpstream >> n;
						vertindexes.push_back(abs(v));
						vertindexes.push_back(abs(vt));
						vertindexes.push_back(abs(n));
					}
					for (int i = 0; i < 3; i++)
					{
						vertexIndices.push_back(vertindexes[i * 3 + 0]);
						uvIndices.push_back(vertindexes[i * 3 + 1]);
						normalIndices.push_back(vertindexes[i * 3 + 2]);
					}
					size_t tmpsize = vertexes.size();
					if (tmpsize > 3)
					{
						for (int i = 3; i < tmpsize; i++)
						{
							vertexIndices.push_back(vertindexes[0]);
							uvIndices.push_back(vertindexes[1]);
							normalIndices.push_back(vertindexes[2]);
							vertexIndices.push_back(vertindexes[(i - 1) * 3 + 0]);
							uvIndices.push_back(vertindexes[(i - 1) * 3 + 1]);
							normalIndices.push_back(vertindexes[(i - 1) * 3 + 2]);
							vertexIndices.push_back(vertindexes[i * 3 + 0]);
							uvIndices.push_back(vertindexes[i * 3 + 1]);
							normalIndices.push_back(vertindexes[i * 3 + 2]);
						}
					}
					isChecked = true;
				}
			}
		}
	}


	size_t tmpsize = vertexIndices.size();
	for (unsigned int i = 0; i < tmpsize; i++)
	{
		if (temp_vertices.size() > 0 && i < vertexIndices.size() && (vertexIndices[i] - 1) >= 0 && (vertexIndices[i] - 1) < temp_vertices.size()) {
			glm::vec3 vertex = temp_vertices[vertexIndices[i] - 1];
			outVertices.push_back(vertex);
		}
		if (temp_normals.size() > 0 && i < normalIndices.size() && (normalIndices[i] - 1) >= 0 && (normalIndices[i] - 1) < temp_normals.size()) {
			glm::vec3 normal = temp_normals[normalIndices[i] - 1];
			outNormals.push_back(normal);
		}
		if (temp_uvs.size() > 0 && i < uvIndices.size() && (uvIndices[i] - 1)>=0 && (uvIndices[i] - 1) < temp_uvs.size()) {
			glm::vec2 vt = temp_uvs[uvIndices[i] - 1];
			outTextures.push_back(vt);
		}
	}

	return 0;
}