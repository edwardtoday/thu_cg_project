#ifndef OBJECT_H_
#define OBJECT_H_

#include "ray.h"

class material 
{
public:
	material() 
	{}
	void setColor(color& _color) 
	{
		m_color = _color;	
	}
	color getColor() 
	{
		return m_color; 
	}
	void setDiffuse(float _diff) 
	{
		m_diff = _diff; 
	}
	void setReflection(float _refl) 
	{
		m_refl = _refl; 
	}
	void setRefraction(float _trans) 
	{
		m_trans = _trans; 
	}
	float getDiffuse() 
	{
		return m_diff; 
	}
	float getReflection() 
	{
		return m_refl; 
	}
	float getSpecular()
	{
		return 1- m_diff; 
	}
	float getRefraction() 
	{
		return m_trans; 
	}
private:
	color m_color;
	float m_refl;
	float m_diff;
	float m_trans;
};

class primitive 
{
public:
	virtual int getType() = 0;
	virtual int intersect(ray& _ray, float& dist) = 0;
	virtual dimension3 getNormal(dimension3 _pos) = 0;
	virtual bool isLight() = 0;
	virtual color getColor(dimension3& pos) = 0;
	material* getMaterial() 
	{
		return &m_material; 
	}
	void setMaterial(material& _mat) 
	{
		m_material = _mat; 
	}
	void setTex() 
	{
		tex = 1; 
	}
	int getTex() 
	{
		return tex; 
	}
protected:
	material  m_material;
	int tex;
};

class sphere: public primitive 
{
public:
	int getType()	
	{
		return 1; 
	}
	sphere(dimension3 _center, float _radius):m_center(_center), m_radius(_radius) 
	{
		m_light = false;
	}
	dimension3&  getCenter() 
	{
		return m_center; 
	}
	dimension3 getNormal(dimension3 _pos) 
	{
		return (_pos - m_center) / m_radius;
	}
	int	intersect(ray& _ray, float& _dist);
	void setLight() 
	{
		m_light = true; 
	}
	bool isLight() 
	{
		return m_light; 
	}
	color getColor(dimension3& pos);
private:
	dimension3 m_center;
	float m_radius;
	bool m_light;
};

class plane : public primitive 
{
public:
	int getType() 
	{
		return 2; 
	}
	plane(dimension3 _normal,float _d) 
	{
		normal = _normal;
		d = _d;
		tex = 0;
	}
	void setTex()
	{
		tex = 1; 
	}
	int intersect(ray& _ray, float& dist);
	dimension3 getNormal(dimension3 _pos) 
	{
		return normal; 
	}
	float getD() 
	{
		return d; 
	}
	bool isLight() 
	{
		return false; 
	}
	color getColor(dimension3& pos);
	
private:
	dimension3 normal;
	float d;
	int tex;
};

class cube: public primitive 
{
public:
	int getType() 
	{
		return 3;
	}
	cube(dimension3* normals, float* scalar) 
	{
		for(int i = 0;i < 6; i++) 
		{
			n[i] = normals[i];
			d[i] = scalar[i];
		}
	}
	bool isLight() 
	{
		return false; 
	}
	int intersect(ray& _ray,float& dist);
	dimension3 getNormal(dimension3 _pos);
	color getColor(dimension3& pos);
private:
	dimension3 n[6];
	float	d[6];
};

#endif
