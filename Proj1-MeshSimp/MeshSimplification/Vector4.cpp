#include <windows.h>
#include <memory.h>
#include "Vector4.h"

using namespace Matrix;
matrix4::matrix4() {
	memset(matrix, 0, sizeof(matrix));
}
matrix4::matrix4(const matrix4 &m) {
	memcpy(matrix, m.matrix, sizeof(matrix));
}
matrix4::matrix4(double *p) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = *(p + i * 4 + j);
}
matrix4 &matrix4::operator=(const matrix4 &m) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = m.matrix[i][j];
	return *this;
}
matrix4 matrix4::operator+(const matrix4 &m) const {
	matrix4 temp;
	memset(temp.matrix, 0, sizeof(temp.matrix));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.matrix[i][j] = matrix[i][j] + m.matrix[i][j];
	return temp;
}
matrix4 matrix4::operator-(const matrix4 &m) const {
	matrix4 temp;
	memset(temp.matrix, 0, sizeof(temp.matrix));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.matrix[i][j] = matrix[i][j] - m.matrix[i][j];
	return temp;
}
Vector4 matrix4::operator*(const Vector4 &v) const {
	Vector4 temp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) 
			temp.points[i] += matrix[i][j] * v.points[j];
	}
	return temp;
}
matrix4 matrix4::operator-() const {
	matrix4 temp;
	memset(temp.matrix, 0, sizeof(temp.matrix));
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp.matrix[i][j] = -matrix[i][j];
	return temp;
}