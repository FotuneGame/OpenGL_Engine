#include "Material.h"

string Material::readShaderFile(const char* filename) {
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