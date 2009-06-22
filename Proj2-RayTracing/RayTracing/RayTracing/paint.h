#ifndef PAINT_H_
#define PAINT_H_

#include "object.h"
#include "bmpInfo.h"

class drawing 
{
public:
	drawing(int width,int height);
	~drawing();
	void clear(unsigned int color);
	BmpData ** getBuffer() 
	{
		return m_buffer; 
	}
	void init(int frame);
	bool render();
	primitive* raytrace(ray& _ray, color& test, int depth,float& dist,float rindex);
private:
	BmpData ** m_buffer;
	int width,height;
	primitive** prim;
	int sx, sy , ex, ey;
	float dx, dy;
	float vx,vy;
	int nPrim;
};

#endif
