#ifndef _VECTOR_4_H_
#define _VECTOR_4_H_

#include <math.h>

namespace Matrix {
	struct Vector4;
	struct matrix4 {
		double matrix[4][4];
		matrix4();
		matrix4(const matrix4 &m);
		matrix4(double *p);
		matrix4 &operator=(const matrix4 &m);
		matrix4 operator+(const matrix4 &m) const;
		matrix4 operator-(const matrix4 &m) const;
		Vector4 operator*(const Vector4 &v) const;
		matrix4 operator-() const;
	};

	const matrix4 Zero;

	struct Vector4 {
		union {
			struct {
				double x;
				double y;
				double z;
				double p;
			};
			double points[4];
		};
		Vector4() : x(0), y(0), z(0), p(0) {}
		Vector4(const Vector4 &v) : x(v.x), y(v.y), z(v.z), p(v.p) {}
		Vector4(double a, double b, double c, double d) : x(a), y(b), z(c), p(d) {}
		Vector4(double *ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]), p(ptr[3]) {}
		Vector4& operator=(const Vector4 &v) {
			x = v.x;
			y = v.y;
			z = v.z;
			p = v.p;
			return *this;
		}
		Vector4 operator+(const Vector4 &v) const 
		{
			return Vector4(x + v.x, y + v.y, z + v.z, p + v.p);
		}
		Vector4 operator-(const Vector4 &v) const 
		{
			return Vector4(x - v.x, y - v.y, z - v.z, p - v.p);
		}
		Vector4 operator*(double l) const 
		{
			return Vector4(x * l, y * l, z * l, p * l);
		}
		Vector4 operator*(matrix4 &m) const {
			Vector4 temp;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) 
					temp.points[i] += points[j] * m.matrix[j][i];
			}
			return temp;
		}
		Vector4 operator/(double l) const 
		{
			return Vector4(x / l, y / l, z / l, p / l);
		}
		double operator*(const Vector4 &v) const {
			return x * v.x + y * v.y + z * v.z + p * v.p;
		}
		matrix4 operator^(const Vector4 &v) const {
			matrix4 temp;
			memset(temp.matrix, 0, sizeof(temp.matrix));
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++) {
					temp.matrix[i][j] = points[i] * v.points[j];
				}
			return temp;
		}
		Vector4 Normalize() {
			*this = *this / p;
			return *this;
		}
		double & operator[](int p) {
			return points[p];
		}
		Vector4 operator-() const {
			return Vector4(-x, -y, -z, -p);
		}
	};
}

#endif//_VECTOR_4_H_