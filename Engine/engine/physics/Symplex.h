#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

//Симплекс объект для GJK
struct Simplex {
public:
	Simplex() : _points({ glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0) }),_size(0) {};

	Simplex& operator=(std::initializer_list<glm::vec3> list)
	{
		_size = 0;

		for (glm::vec3 point : list)
			_points[_size++] = point;

		return *this;
	}

	void push_front(glm::vec3 point)
	{
		_points = { point, _points[0], _points[1], _points[2] };
		_size = std::min(_size + 1, 4);
	}

	glm::vec3& operator[](int i) { 
		return _points[i];
	}
	const size_t size() {
		return _size;
	}

	const auto begin() {
		return _points.begin();
	}
	const auto end() {
		return _points.end() - (4 - _size);
	}
private:
	std::array<glm::vec3, 4> _points;
	int _size;
};