#include "Material.h"

string Material::readShaderFile(const char* filename) {
	string res;
	ifstream file(filename, ios::in);
	if (file.is_open())
	{
		std::stringstream sstr; // ����� ��� ������
		sstr << file.rdbuf(); // ��������� ����
		res = sstr.str(); //�������� ������ �� ������
		file.close(); //��������� ����
	}
	return res;
}