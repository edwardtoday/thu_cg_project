#include "object.h"
#include <stdio.h>
#include <cmath>
#include <cstdlib>

int sphere::intersect(ray& _ray, float& _dist) 
{
	dimension3 v = _ray.getOrigin() - m_center;
	float b = -1 * v.dot(_ray.getDirection());
	float det = (b * b) - v.dot(v) + m_radius * m_radius;
	int	inter = 0;
	if( det > 0) 
	{
		det = sqrt(det);
		float root1 = b + det;
		float root2 = b - det;
		if( root1 > 0) 
		{
			if(root2 < 0) 
			{
				if(root1 < _dist) 
				{
					_dist = root1;
					inter = -1;
				}
			}
			else 
			{
				if(root2 < _dist) 
				{
					_dist = root2;
					inter = 1;
				}
			}
		}
	}
	return inter;
}

color sphere::getColor(dimension3 &pos) 
{
	return this->getMaterial()->getColor();
}

int plane::intersect(ray &_ray, float &dist) 
{
	float temp1 = normal.dot( _ray.getOrigin());
	float temp2 = normal.dot( _ray.getDirection());
	float temp = -1 * (temp1 + d) / temp2 ;
	if(temp > 0) 
	{
		if(temp < dist) 
		{
			dist = temp;
			return 1;
		}	
	}
	return 0;
}

color plane::getColor(dimension3& pos) 
{
	if(this->getTex() == 1)
	{
		int x = (int)(pos.m_x/2);
		int z = (int)pos.m_z;
		if((x+z)%2 == 0)
		{
			return this->getMaterial()->getColor();
		}
		else
		{
			return (float)0.9*(this->getMaterial()->getColor());
		}
	}
	else
	{
		return this->getMaterial()->getColor();
	}
}

int cube::intersect(ray &_ray, float &dist) 
{
	float fmax = -10000.0f;
	float fmin = 10000.0f;
	float s;
	float alpha;
	for(int i =0 ;i < 6;i ++) 
	{
		s = _ray.getDirection().dot(n[i]);
		if(s==0) 
		{
			if(_ray.getOrigin().dot(n[i]) > d[i]) return 0;
			continue;
		}
		alpha = (d[i] - _ray.getOrigin().dot(n[i])) / s;
		if(_ray.getDirection().dot(n[i]) < 0)
		{
			if(alpha > fmax) 
			{
				if(alpha > fmin) 
				{
					return 0;
				}
				fmax = alpha;
			}
		}
		else 
		{
			if(alpha < fmin) 
			{
				if(alpha < 0 || alpha < fmax) 
				{
					return 0;
				}
				fmin = alpha;
			}
		}
	}
	if(fmax > 0) alpha = fmax;
	else alpha = fmin;
	dist = alpha;
	return 1;
}

color cube::getColor(dimension3 &pos) 
{
	return this->getMaterial()->getColor();
}

dimension3 cube::getNormal(dimension3 _pos) 
{
	int k = 0;
	float min = 10000.0f;
	for(int i = 0;i < 6; i++) 
	{
		float temp = abs(n[i].dot(_pos) - d[i]);
		if(min > temp) 
		{
			min = temp;
			k = i;
		}
	}
	return n[k];
}
