#ifndef TRIDIMENSION_H_
#define TRIDIMENSION_H_

#include <math.h>

class dimension3 
{
public:
	dimension3() : m_x(0),m_y(0),m_z(0) 
	{}
	dimension3(float _x, float _y, float _z) 
	{
		m_x = _x;
		m_y = _y;
		m_z = _z;
	}
	float dot(dimension3& v) 
	{ 
		return m_x*v.m_x + m_y*v.m_y + m_z*v.m_z; 
	}
	dimension3 cross(dimension3 v) 
	{ 
		return dimension3( m_y * v.m_z - m_z * v.m_y, m_z * v.m_x - m_x * v.m_z, m_x * v.m_y - m_y * v.m_x ); 
	}
	float getLength() 
	{
		return  sqrtf(m_x*m_x + m_y * m_y + m_z * m_z);
	}
	dimension3 getNormal() 
	{
		float temp = 1/sqrtf(m_x*m_x + m_y*m_y + m_z*m_z);
		return dimension3(m_x * temp, m_y * temp, m_z * temp);
	}
	float m_x, m_y, m_z;
	dimension3 operator -() 
	{
		return dimension3( -1 * m_x,-1 * m_y,-1 * m_z );
	} 
	friend dimension3 operator + (const dimension3& left, const dimension3& right) 
	{
		return dimension3(left.m_x + right.m_x,left.m_y + right.m_y, left.m_z + right.m_z);
	}
	friend dimension3 operator - (const dimension3& left, const dimension3& right) 
	{
		return left + ( -1 * right);
	}
	friend dimension3 operator / (const dimension3& left, float f) 
	{
		return dimension3(left.m_x / f, left.m_y /f ,left.m_z / f);
	}
	friend dimension3 operator * (float f, const dimension3& right) 
	{
		return dimension3(f * right.m_x, f * right.m_y, f * right.m_z);
	}
	friend dimension3 operator * (const dimension3& left, const dimension3& right) 
	{
		return dimension3(left.m_x*right.m_x,left.m_y*right.m_y,left.m_z*right.m_z);
	}
};

typedef	dimension3 color;

#endif
