#include"materials.h"
#include<math.h>
#include<iostream>
using namespace std;

Point operator + (const Point a,const Point b) 
{
	Point res;
	res.x=a.x+b.x;
	res.y=a.y+b.y;
	res.z=a.z+b.z;
	return res;
}

Point operator - (const Point a,const Point b)
{
	Point res;
	res.x=a.x-b.x;
	res.y=a.y-b.y;
	res.z=a.z-b.z;
	return res;
}

Point operator * (const Point a,double factor) 
{
	Point res;
	res.x=a.x*factor;
	res.y=a.y*factor;
	res.z=a.z*factor;
	return res;
}

Point operator / (const Point a,double factor)
{
	Point res;
	res.x=a.x/factor;
	res.y=a.y/factor;
	res.z=a.z/factor;
	return res;
}

Point& Point::operator =( const Point& v)
{
	this->x=v.x;
	this->y=v.y;
	this->z=v.z;
	return (*this);
}

bool operator !=(const Point a,const Point b)
{
	if(a.x!=b.x||a.y!=b.y||a.z!=b.z)
		return true;
	return false;
}

bool operator ==(const Point a,const Point b)
{
	if(a.x==b.x&&a.y==b.y&&a.z==b.z)
		return true;
	return false;	
}

Point XMul(const Point p,const Point q)
{
	Point result;
	double x=p.y*q.z-q.y*p.z;
	double y=p.x*q.z-q.x*p.z;
	double z=p.x*q.y-q.x*p.y;
	double temp=sqrt(x*x+y*y+z*z);
	result.x=x/temp;
	result.y=y/temp;
	result.z=z/temp;
	return result;
}

double Mul(const Point p,const Point q)
{
	return (p.x*q.x+p.y+q.y+p.z*q.z);
}

Edge& Edge::operator =( const Edge& e)
{
	this->v1=e.v1;
	this->v2=e.v2;
	this->choice=e.choice;
	this->distance=e.distance;
	return (*this);
}

double p2pDis(const Point p,const Point q)
{
	return sqrt(pow(p.x-q.x,2)+pow(p.y-q.y,2)+pow(p.z-q.z,2));
}
