#include "Collider.h"


Collider::Collider(Transform* transform) {
	_transform = transform;
	_color = { 0,1,0 };
	_model = new Model();
}
Collider::~Collider() {
	if (_model)
		delete _model;
}



int Collider::render(Camera* camera) {
	if (!_transform || !camera)return -3;

	camera->render();
	glm::mat4 M = _transform->getMatrix();

	M = glm::translate(M, settings.offset);
	M = glm::rotate(M, glm::radians(settings.offsetAngles.x), { 1,0,0 });
	M = glm::rotate(M, glm::radians(settings.offsetAngles.y), { 0,1,0 });
	M = glm::rotate(M, glm::radians(settings.offsetAngles.z), { 0,0,1 });
	M = glm::scale(M, settings.scale);


	return _model->render(M, camera->getTransform()->getMatrix(), camera->getProjection(), camera->getTransform()->getPosition());

	return 0;
}



void Collider::loadColliderModelFromFile(const char* filename) {
	if (!_model || !filename) return;

	std::vector<glm::vec3> outVertices;
	std::vector<glm::vec2> outTextures;
	std::vector<glm::vec3> outNormals;

	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	if (!_verticesModel.size())
		_verticesModel.clear();
	_model->getMesh()->loadModelFromFile(filename, outVertices,outTextures,outNormals,_verticesModel, temp_uvs, temp_normals);
	_model->getMaterial()->loadColor(_color);
	_model->setMode(GL_LINE_LOOP);
	if(!_vertices.size())
		_vertices.clear();
	for (glm::vec3 vertex : _verticesModel) {
		_vertices.push_back(vertex);
	}
}

Model* Collider::getModel() {
	return _model;
}


//Метод для вершин выпуклого n угольника
void Collider::updateTransformToVertices() {
	glm::mat4 M = _transform->getMatrix();
	M = glm::translate(M, settings.offset);
	M = glm::rotate(M, settings.offsetAngles.x, {1,0,0});
	M = glm::rotate(M, settings.offsetAngles.y, { 0,1,0 });
	M = glm::rotate(M, settings.offsetAngles.z, { 0,0,1 });
	M = glm::scale(M, settings.scale);
	for (int i = 0; i < _verticesModel.size(); i++) {
		_vertices[i] = M * glm::vec4(_verticesModel[i].x, _verticesModel[i].y, _verticesModel[i].z, 1.f);
	}
}



CollisionPoints Collider::collision(Collider* collider_b) {
	CollisionPoints answer;
	if (!_verticesModel.size() || !collider_b->_verticesModel.size()) return answer;

	Simplex points;
	bool isCollision = GJK(collider_b, points);

	if (isCollision) {
		answer = EPA(points, collider_b);
	}
	return answer;
}








//Алгоритм GJK И EPA
//Поиск наидельнейшей точки модели в модели
glm::vec3 Collider::findFurthestPoint(glm::vec3 direction) {
	glm::vec3 maxPoint;
	float maxDistance = -FLT_MAX;

	for (glm::vec3 vertex : _vertices) {
		float distance = glm::dot(vertex, direction);
		if (distance > maxDistance) {
			maxDistance = distance;
			maxPoint = vertex;
		}
	}

	return maxPoint;
}


//Смотрим разность минковского если разность для выпуклых многоугольников и ищем при пересечении что мн-во минковского иммеет начало координат
glm::vec3 Collider::supportDistance(Collider* collider_b, glm::vec3 direction)
{
	glm::vec3 a = findFurthestPoint(direction);
	glm::vec3 b = collider_b->findFurthestPoint(-direction);
	return a - b;
}


bool Collider::GJK(Collider* collider_b, Simplex& points) {
	// Получить начальную точку опоры в любом направлении
	glm::vec3 support = supportDistance(collider_b, _transform->getPosition()- collider_b->_transform->getPosition());

	// Симплекс - это массив точек, максимальное количество которых равно 4
	points.push_front(support);

	// Новое направление - к источнику
	glm::vec3 direction = -support;

	while (true) {
		support = supportDistance(collider_b, direction);

		if (glm::dot(support, direction) <= 0) {
			return false; // нет коллизии
		}

		points.push_front(support);
		if (nextSimplex(points, direction)) {
			return true;
		}
	}
}



bool Collider::nextSimplex(Simplex& points, glm::vec3& direction){
	switch (points.size()) {
	case 2: return lineSymplex(points, direction);
	case 3: return triangleSimplex(points, direction);
	case 4: return tetrahedronSymplex(points, direction);
	}

	//если симплекс точка или > 3-х измерений
	return false;
}

bool Collider::sameDirection(const glm::vec3& direction, const glm::vec3& ao)
{
	return dot(direction, ao) > 0;
}

bool Collider::lineSymplex(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (sameDirection(ab, ao)) {
		direction = cross(cross(ab, ao), ab);
	}

	else {
		points = { a };
		direction = ao;
	}

	return false;
}


bool Collider::triangleSimplex(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = cross(ab, ac);

	if (sameDirection(cross(abc, ac), ao)) {
		if (sameDirection(ac, ao)) {
			points = { a, c };
			direction = cross(cross(ac, ao), ac);
		}

		else {
			return lineSymplex(points = { a, b }, direction);
		}
	}

	else {
		if (sameDirection(cross(ab, abc), ao)) {
			return lineSymplex(points = { a, b }, direction);
		}

		else {
			if (sameDirection(abc, ao)) {
				direction = abc;
			}

			else {
				points = { a, c, b };
				direction = -abc;
			}
		}
	}

	return false;
}


bool Collider::tetrahedronSymplex(Simplex& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];
	glm::vec3 d = points[3];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ad = d - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = cross(ab, ac);
	glm::vec3 acd = cross(ac, ad);
	glm::vec3 adb = cross(ad, ab);

	if (sameDirection(abc, ao)) {
		return triangleSimplex(points = { a, b, c }, direction);
	}

	if (sameDirection(acd, ao)) {
		return triangleSimplex(points = { a, c, d }, direction);
	}

	if (sameDirection(adb, ao)) {
		return triangleSimplex(points = { a, d, b }, direction);
	}

	return true;
}






CollisionPoints Collider::EPA(Simplex& simplex, Collider* collider_b) {
	std::vector<glm::vec3> polytope(simplex.begin(), simplex.end());
	std::vector<size_t> faces = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};

	// список: vec4(обычный, расстояние), индекс: минимальное расстояние [normals, minFace]
	auto res = getFaceNormals(polytope, faces);


	glm::vec3  minNormal;
	float minDistance = FLT_MAX;

	while (minDistance == FLT_MAX) {
		minNormal = glm::vec3(res.first[res.second].x, res.first[res.second].y, res.first[res.second].z);
		minDistance = res.first[res.second].w;

		glm::vec3 support = supportDistance(collider_b, minNormal);
		float sDistance = dot(minNormal, support);

		if (abs(sDistance - minDistance) > 0.001f) {
			minDistance = FLT_MAX;
			std::vector<std::pair<size_t, size_t>> uniqueEdges;

			for (size_t i = 0; i < res.first.size(); i++) {
				if (glm::dot(glm::vec3(res.first[i]), support) > glm::dot(glm::vec3(res.first[i]), polytope[faces[i * 3]]) ) {
					size_t f = i * 3;

					addIfUniqueEdge(uniqueEdges, faces, f, f + 1);
					addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
					addIfUniqueEdge(uniqueEdges, faces, f + 2, f);

					faces[f + 2] = faces.back(); faces.pop_back();
					faces[f + 1] = faces.back(); faces.pop_back();
					faces[f] = faces.back(); faces.pop_back();

					res.first[i] = res.first.back(); // pop-erase
					res.first.pop_back();

					i--;
				}
			}
			std::vector<size_t> newFaces;
			for (auto res_uniq : uniqueEdges) {
				newFaces.push_back(res_uniq.first);
				newFaces.push_back(res_uniq.second);
				newFaces.push_back(polytope.size());
			}

			polytope.push_back(support);

			// НОВЫЙ список: vec4(обычный, расстояние), индекс: минимальное расстояние [newNormals, newMinFace]
			auto res_new = getFaceNormals(polytope, newFaces);
			float oldMinDistance = FLT_MAX;
			for (size_t i = 0; i < res.first.size(); i++) {
				if (res.first[i].w < oldMinDistance) {
					oldMinDistance = res.first[i].w;
					res.second = i;
				}
			}

			if (res_new.first[res_new.second].w < oldMinDistance) {
				res.second = res_new.second + res.first.size();
			}

			faces.insert(faces.end(), newFaces.begin(), newFaces.end());
			res.first.insert(res.first.end(), res_new.first.begin(), res_new.first.end());
		}
	}

	CollisionPoints points;

	points.normal = minNormal;
	points.depth = minDistance + 0.001f;
	points.isCollision = true;

	return points;
}



std::pair<std::vector<glm::vec4>, size_t> Collider::getFaceNormals(const std::vector<glm::vec3>& polytope,const std::vector<size_t>& faces)
{
	std::vector<glm::vec4> normals;
	size_t minTriangle = 0;
	float  minDistance = FLT_MAX;

	for (size_t i = 0; i < faces.size(); i += 3) {
		glm::vec3 a = polytope[faces[i]];
		glm::vec3 b = polytope[faces[i + 1]];
		glm::vec3 c = polytope[faces[i + 2]];

		glm::vec3 normal = glm::cross(b - a, c - a);
		if (glm::length(normal) != 0)
			normal = glm::normalize(normal);
		float distance = glm::dot(normal, a);

		if (distance < 0) {
			normal *= -1;
			distance *= -1;
		}

		normals.emplace_back(normal, distance);

		if (distance < minDistance) {
			minTriangle = i / 3;
			minDistance = distance;
		}
	}

	return { normals, minTriangle };
}


void Collider::addIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges,const std::vector<size_t>& faces,size_t a,size_t b)
{
	auto reverse = std::find(                       //      0--<--3
		edges.begin(),                              //     / \ B /   A: 2-0
		edges.end(),                                //    / A \ /    B: 0-2
		std::make_pair(faces[b], faces[a]) //   1-->--2
	);

	if (reverse != edges.end()) {
		edges.erase(reverse);
	}

	else {
		edges.emplace_back(faces[a], faces[b]);
	}
}