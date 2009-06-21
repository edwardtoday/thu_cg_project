#ifndef RAY_H_
#define RAY_H_

#include "triDimension.h"

class ray 
{
public:
	ray(): m_origin(dimension3(0,0,0)), m_direction(dimension3(0,0,0)) 
	{}
	ray(dimension3& _origin, dimension3& _direction ): m_origin(_origin), m_direction(_direction) 
	{}
	void setOrigin(dimension3& _origin) 
	{
		m_origin = _origin ; 
	}
	void setDirection(dimension3& _direction) 
	{ 
		m_direction = _direction;
	}
	dimension3& getOrigin() 
	{
		return m_origin; 
	}
	dimension3& getDirection() 
	{
		return m_direction; 
	}
private:
	dimension3  m_origin;
	dimension3  m_direction;
};

#endif
