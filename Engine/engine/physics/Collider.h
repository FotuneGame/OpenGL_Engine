#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <vector>
#include "../camera/Camera.h"
#include "../component/Transform.h"
#include "../object/Model.h"
#include "Symplex.h"



struct ColliderSettings {
	glm::vec3 scale = { 1,1,1 };
	glm::vec3 offsetAngles = { 0,0,0 };
	glm::vec3 offset = { 0,0,0 };
};


struct CollisionPoints {
	glm::vec3 normal = {0,0,0};
	float depth =0.f; //дистанция b a самых удалённых точек минковского
	bool isCollision = false;
};


class Collider {
public:
	Collider(Transform* transform);
	~Collider();

	int render(Camera* camera);
	void updateTransformToVertices();
	void loadColliderModelFromFile(const char* filename);
	CollisionPoints collision(Collider* collider_b);

	ColliderSettings settings;
	Model* getModel();
private:
	Model* _model;
	Transform* _transform;
	glm::vec3 _color;
	std::vector<glm::vec3> _verticesModel;
	std::vector<glm::vec3> _vertices;


	bool GJK(Collider* collider_b, Simplex& simplex);
	CollisionPoints EPA(Simplex& simplex, Collider* collider_b);
	//GJK
	glm::vec3 findFurthestPoint(glm::vec3 direction);
	glm::vec3 supportDistance(Collider* collider_b, glm::vec3 direction);
	bool nextSimplex(Simplex& points, glm::vec3& direction);
	bool sameDirection(const glm::vec3& direction, const glm::vec3& ao);
	bool lineSymplex(Simplex& points, glm::vec3& direction);
	bool triangleSimplex(Simplex& points, glm::vec3& direction);
	bool tetrahedronSymplex(Simplex& points, glm::vec3& direction);
	//EPA
	std::pair<std::vector<glm::vec4>, size_t> getFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<size_t>& faces);
	void addIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);
};
