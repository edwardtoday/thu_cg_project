#ifndef _VECTOR_3_H_
#define _VECTOR_3_H_

#include <math.h>
#include <gl/gl.h>
#include <GL/glu.h>

#define _VECTOR_3_H_SQR(x) (x) * (x)
#pragma warning(disable : 4244)

namespace Specular {
	const double PI = 3.1415927;
	struct Vector3 {
		union {
			struct {
				double x;
				double y;
				double z;
			};
			double points[3];
		};
		Vector3() : x(0), y(0), z(0) {}
		Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}
		Vector3(double a, double b, double c) : x(a), y(b), z(c) {}
		Vector3(double *ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]) {}
		Vector3& operator=(const Vector3 &v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
		Vector3 operator+(const Vector3 &v) const 
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}
		Vector3 operator-(const Vector3 &v) const 
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}
		Vector3 operator*(double l) const 
		{
			return Vector3(x * l, y * l, z * l);
		}
		Vector3 operator/(double l) const 
		{
			return Vector3(x / l, y / l, z / l);
		}
		double operator*(const Vector3 &v) const {
			return x * v.x + y * v.y + z * v.z;
		}
		Vector3 operator^(const Vector3 &v) const {
			return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		}
		double Length() const {
			double temp = sqrt(_VECTOR_3_H_SQR(x) + _VECTOR_3_H_SQR(y) + _VECTOR_3_H_SQR(z));
			return (temp != 0) ? temp : 1.0;
		}
		Vector3 Normalize() {
			*this = *this * (1.0 / this->Length());
			return *this;
		}
		double & operator[](int p) {
			return points[p];
		}
		Vector3 operator-() const {
			return Vector3(-x, -y, -z);
		}
	};
	class Watch {
	public:
		Vector3 Eye;
		Vector3 Top;
		double Distance;
	public:
		Watch() {
			Eye.x = Eye.y = Eye.z;
			Top.x = Top.y = Top.z = 0;
			Distance = 0;
		}
		~Watch() {		
			Eye.x = Eye.y = Eye.z;
			Top.x = Top.y = Top.z = 0;
			Distance = 0;
		}
		Watch(Vector3 eye, Vector3 top) {
			this->SetPosition(eye);
			this->SetTop(top);
		}
		Watch(double ex, double ey, double ez, double tx, double ty, double tz) {
			this->SetPosition(Vector3(ex, ey, ez));
			this->SetTop(Vector3(tx, ty, tz));
		}
		void LookAt() {
			float lightpos0[] = {Eye.x, Eye.y, Eye.z, 0.0};
			glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
			gluLookAt(Eye.x, Eye.y, Eye.z, 0, 0, 0, Top.x, Top.y, Top.z);
		}
		void SetPosition(Vector3 eye) {
			Eye = eye;
			Distance = Eye.Length();			
		}
		void SetTop(Vector3 top) {
			Top = top;
			Top.Normalize();
		}
		void MoveUp(double degree) {
			double alpha = degree / 180 * PI;
			Vector3 temp = Eye;
			Eye = Eye * cos(alpha) + Top * Distance * sin(alpha);
			Top = -temp / temp.Length() * sin(alpha) + Top * cos(alpha);
		}
		void MoveDown(double degree) {
			double alpha = degree / 180 * PI;
			Vector3 temp = Eye;
			Eye = Eye * cos(alpha) - Top * Distance * sin(alpha);
			Top = temp / temp.Length() * sin(alpha) + Top * cos(alpha);
		}
		void MoveLeft(double degree) {
			Vector3 Left = (Eye ^ Top).Normalize();
			double alpha = degree / 180 * PI;
			Eye = Eye * cos(alpha) + Left * Distance * sin(alpha);
		}
		void MoveRight(double degree) {
			Vector3 Left = (Eye ^ Top).Normalize();
			double alpha = degree / 180 * PI;
			Eye = Eye * cos(alpha) - Left * Distance * sin(alpha);
		}
		void MoveTowards(double dis) {
			Distance -= dis;
			Eye.Normalize();
			Eye = Eye * Distance;
			Distance = fabs(Distance);
		}
		Vector3 GetEye() const {
			return Eye;
		}
		Vector3 GetTop() const {
			return Top;
		}
	};
	class Perspective {
		Vector3 Eye;
		Vector3 Direction;
		Vector3 Up;
	public:
		Perspective() {
			Eye.x = Eye.y = Eye.z = 0;
			Direction.x = Direction.y = Direction.z = 0;
			Up.x = Up.y = Up.z = 0;
		}
		Perspective(const Perspective &psp) {
			Eye = psp.Eye;
			Direction = psp.Direction;
			Up = psp.Up;
		}
		Perspective(Vector3 e, Vector3 d, Vector3 u) : Eye(e), Direction(d), Up(u) {
			Direction.Normalize();
			Up.Normalize();
		}
		Perspective(double eyex, double eyey, double eyez, 
			double dirx, double diry, double dirz, 
			double upx, double upy, double upz) :
			Eye(eyex, eyey, eyez), Direction(dirx, diry, dirz), Up(upx, upy, upz) 
		{
			Direction.Normalize();
			Up.Normalize();
		}
		void SetEye(const Vector3 &v) {
			Eye = v;
		}
		void SetDirection(const Vector3 &v) {
			Direction = v;
		}
		void SetUp(const Vector3 &v) {
			Up = v;
		}
		void SetCenter(const Vector3 &v) {
			Direction = v - Eye;
		}
		void LookAt() {
			Vector3 Center = Eye + Direction;
			gluLookAt(Eye.x, Eye.y, Eye.z, Center.x, Center.y, Center.z, Up.x, Up.y, Up.z);
		}
		void TurnUp(double degree) {
			Vector3 tempUp = Up * cos(degree / 180 * PI) - Direction * sin(degree / 180 * PI);
			Vector3 tempDirection = Direction * cos(degree / 180 * PI) + Up * sin(degree / 180 * PI);
			Up = tempUp;
			Direction = tempDirection;
		}
		void TurnDown(double degree) {
			Vector3 tempUp = Up * cos(degree / 180 * PI) + Direction * sin(degree / 180 * PI);
			Vector3 tempDirection = Direction * cos(degree / 180 * PI) - Up * sin(degree / 180 * PI);
			Up = tempUp;
			Direction = tempDirection;
		}
		void TurnLeft(double degree) {
			Vector3 Left = Up ^ Direction;
			Direction = Direction * cos(degree / 180 * PI) + Left * sin(degree / 180 * PI);
		}
		void TurnRight(double degree) {
			Vector3 Right = Direction ^ Up;
			Direction = Direction * cos(degree / 180 * PI) + Right * sin(degree / 180 * PI);
		}
		void RotateCW(double degree) {
			Vector3 Right = Direction ^ Up;
			Up = Up * cos(degree / 180 * PI) + Right * sin(degree / 180 * PI);
		}
		void RotateCCW(double degree) {
			Vector3 Left = Up ^ Direction;
			Up = Up * cos(degree / 180 * PI) + Left * sin(degree / 180 * PI);
		}
		void MoveUp(double distance)	{
			Eye = Eye + Up * distance;
		}
		void MoveDown(double distance) {
			Eye = Eye - Up * distance;
		}
		void MoveLeft(double distance) {
			Vector3 Left = Up ^ Direction;
			Eye = Eye + Left * distance;
		}
		void MoveRight(double distance) {
			Vector3 Right = Direction ^ Up;
			Eye = Eye + Right * distance;
		}
		void MoveTowards(double distance) {
			Eye = Eye + Direction * distance;
		}
		void MoveBackwards(double distance) {
			Eye = Eye - Direction * distance;
		}
		Vector3 GetEye() const 
		{
			return Eye;
		}
		Vector3 GetDirection() const 
		{
			return Direction;
		}
		Vector3 GetUp() const
		{
			return Up;
		}
		Vector3 GetCenter() const
		{
			return Eye + Direction;
		}
	};
}

#endif//_VECTOR_3_H_