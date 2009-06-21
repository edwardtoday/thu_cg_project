#include <stdio.h>
#include "paint.h"

drawing::drawing(int _width, int _height) 
{
	width = _width;
	height = _height;
	m_buffer = new BmpData* [_height];
	int i;
	for(i=0;i<_height;i++)
	{
		m_buffer[i]=new BmpData [_width];
	}
}

drawing::~drawing() 
{
	int i;
	for(i=0;i<height;i++)
	{
		delete m_buffer[i];
	}
	delete []m_buffer;
}

void drawing::clear(unsigned color) 
{
	int i,j;
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			m_buffer[i][j].rgbBlue = color;
			m_buffer[i][j].rgbGreen = color;
			m_buffer[i][j].rgbRed = color;
		}
}

void drawing::init() 
{
	nPrim = 9;
	prim = new primitive* [nPrim];

	prim[0] = new plane(dimension3(0,1.0f,0),5.0f);
	prim[0]->getMaterial()->setDiffuse(1.0f);
	prim[0]->getMaterial()->setReflection(0.6f);
	prim[0]->getMaterial()->setColor(color(0.2f,0.2f,0.2f/*0.4f,0.3f,0.3f*/));
	prim[0]->getMaterial()->setRefraction(0);
	prim[0]->setTex();

	prim[1] = new sphere( dimension3( 3.0f, 4.0f, -3.0f ), 0.01f);
	prim[1]->getMaterial()->setColor(color(1.0f,1.0f,1.0f));
	((sphere*)prim[1])->setLight();
	prim[2] = new sphere( dimension3(-3.0f,5.0f,-1.0f),0.01f);
	prim[2]->getMaterial()->setColor(color(1.0f,1.0f,1.0f));
	((sphere*)prim[2])->setLight();

	prim[3] = new sphere( dimension3( 1.8f, -1.0f, 3.0f ), 2.5f);
	prim[3]->getMaterial()->setColor(color(0.9f,1.0f,1.0f));
	prim[3]->getMaterial()->setDiffuse(0.7f);
	prim[3]->getMaterial()->setReflection(0.4f);
	prim[3]->getMaterial()->setDiffuse(0.0f);
	prim[3]->getMaterial()->setRefraction(1.25f);

	prim[4] = new sphere(dimension3(-7.7f,-1.8f,9.0f),2);
	prim[4]->getMaterial()->setDiffuse(0.1f);
	prim[4]->getMaterial()->setReflection(1.0f);
	prim[4]->getMaterial()->setColor(color(0.7f,0.7f,1.0f));
	prim[4]->getMaterial()->setRefraction(0);

	prim[5] = new sphere(dimension3(-3.0f,-1.8f,5.5f),1.5f);
	prim[5]->getMaterial()->setDiffuse(0.5f);
	prim[5]->getMaterial()->setReflection(0.5f);
	prim[5]->getMaterial()->setColor(color(0.9f,1.0f,1.0f));
	prim[5]->getMaterial()->setRefraction(0);

	dimension3 d[6];
	d[0] = dimension3(0,1,0).getNormal();
	d[1] = dimension3(0,-1,0).getNormal();
	d[2] = dimension3(1,0,0).getNormal();
	d[3] = dimension3(0,0,1).getNormal();
	d[4] = dimension3(-1,0,0).getNormal();
	d[5] = dimension3(0,0,-1).getNormal();
	float s[6] = {-3.5f,5.0f,-2.7f,4.8f,4.7f,-4.0f};
	prim[6] = new cube(d,s);
	prim[6]->getMaterial()->setDiffuse(0.8f);
	prim[6]->getMaterial()->setReflection(0.5f);
	prim[6]->getMaterial()->setColor(color(0.9f,0.9f,0.9f));
	prim[6]->getMaterial()->setRefraction(0);

	prim[7] = new plane(dimension3(0.0f,0.0f,-1.0f),10.0f);
	prim[7]->getMaterial()->setDiffuse(0.2f);
	prim[7]->getMaterial()->setReflection(0.8f);
	prim[7]->getMaterial()->setColor(color(0.5f,0.1f,0.3f));
	prim[7]->getMaterial()->setRefraction(0);

	prim[8] = new sphere(dimension3(5.0f,-2.0f,5.5f),1.5f);
	prim[8]->getMaterial()->setDiffuse(0.5f);
	prim[8]->getMaterial()->setReflection(0.5f);
	prim[8]->getMaterial()->setColor(color(0.9f,0.6f,0.0f));
	prim[8]->getMaterial()->setRefraction(0);

	sx = -4 ; ex = 4;
	sy = -3 ; ey = 3;
	dx = (float)(ex - sx) / (float)width;
	dy = (float)(sy - ey) / (float)height;
}

bool drawing::render() 
{
	int x,y;
    vy = (float)ey;
	dimension3 eye(0,0,-5);
	for(y = 0; y < height; y++) 
	{
		vx = (float)sx;
		for(x = 0; x < width; x++) 
		{
			color  test(0,0,0);
			dimension3 dir  = dimension3(vx,vy,0) - eye;
			dir = dir.getNormal();
			ray r(eye, dir);
			float dist;
			primitive* prim = raytrace(r,test,1,dist,1.0f);
			int red = (int)(test.m_x * 256);
			int green = (int)(test.m_y * 256);
			int blue = (int)(test.m_z * 256);
			if(red > 255) red = 255;
			if(green > 255) green = 255;
			if(blue > 255) blue = 255;
			m_buffer[height-1-y][width-1-x].rgbBlue = (BYTE)blue;
			m_buffer[height-1-y][width-1-x].rgbGreen =(BYTE)green;
			m_buffer[height-1-y][width-1-x].rgbRed = (BYTE)red;
			vx += dx;
		}
		vy += dy;
	}
	return true;
}

primitive* drawing::raytrace(ray& _ray, color& test, int depth, float& dist,float rindex) 
{
	if(depth > 6) return 0;
	dist = 100000.0f;
	dimension3 v;
	primitive* p = 0;
	int result;
	int position;
	for(int i =0 ;i < nPrim; i++) 
	{
		result = prim[i]->intersect(_ray, dist);
		if( result != 0) 
		{
			p = prim[i];
			position = result;
		}
	}
	if(!p) 
	{
		return 0;
	}
	if(p->isLight()) 
	{
			test = color(1,1,1);
	}
	else
	{
			v = _ray.getOrigin() + dist * _ray.getDirection();
			for(int i =0 ; i< nPrim; i++) 
			{
				if(prim[i]->isLight()) 
				{
					dimension3 l = ((sphere*)prim[i])->getCenter() - v;
					dimension3 n = p->getNormal(v);
					float shadow = 1.0;
					float ldist = l.getLength();
					l = l.getNormal();
					ray peeler = ray(v + 0.001f * l,l);
					for(int j = 0;j < nPrim; j++) 
					{
						if(prim[j]->isLight() == false && prim[j]->intersect(peeler,ldist)== 1) 
						{
							if(prim[j]->getMaterial()->getRefraction()>0)
							{
								shadow = (float)0.8;
							}
							else
							{
								shadow = 0;
								break;
							}
						}
					}
					if(shadow > 0) 
					{
						if(p->getMaterial()->getDiffuse() >0) 
						{
							float dot = n.dot(l);
							if(dot >0) 
							{
								float diff = dot * p->getMaterial()->getDiffuse()* shadow;
								test = test + diff * p->getColor(v) * prim[i]->getMaterial()->getColor();
							}
						}
						if(p->getMaterial()->getSpecular() > 0) 
						{
							dimension3 r = 2.0f * l.dot(n) * n - l;
							float dot = r.dot((-1) * _ray.getDirection());
							if(dot > 0) 
							{
								float spec = powf(dot,20) * p->getMaterial()->getSpecular() * shadow;
								test = test + spec * p->getColor(v) * prim[i]->getMaterial()->getColor();
							}
						}
					}
				}
			}
			if(p->getMaterial()->getReflection() > 0) 
			{
				dimension3 n = p->getNormal(v);
				dimension3 r = _ray.getDirection() - 2.0f *  n.dot(_ray.getDirection()) * n;
				if(depth < 6) 
				{
					color ref(0,0,0);
					float dist1;
					raytrace(ray(v + 0.001f * r, r.getNormal()),ref,depth + 1,dist1,rindex);
					float refl = p->getMaterial()->getReflection();
					test = test + refl * p->getColor(v) * ref;
				}
			}
			if(  p->getMaterial()->getRefraction() >0 && depth < 6) 
			{
				float trans = p->getMaterial()->getRefraction();
				dimension3 n =  (float)position * p->getNormal(v);
				dimension3 d = (-1) * _ray.getDirection();
				if(position == -1) trans = 1 /trans;
				dimension3	t_lat = 1 / trans * (d.dot(n) *n - d);
				float sinsq = t_lat.dot(t_lat);
				if(sinsq <= 1) 
				{
					dimension3 t = t_lat - sqrtf(1- sinsq) * n;
					color refr(0,0,0);
					float dist1;
					raytrace(ray(v + 0.001f * t,t),refr,depth+1,dist1,trans);
					test = test + (float)0.9 * refr;
				}
			}
			
	}
	return p;
}
