#ifndef MATERIALS_H_
#define MATERIALS_H_

#include<vector>
using namespace std;

class Point
{
public:
	double x,y,z;
	int index;
	bool hasRemoved;
	vector<int> tris;
	double matrix[4][4];
public:
	Point()
	{
		x=0;y=0;z=0;
		index=0;
		hasRemoved=false;
	}
	Point& operator =( const Point&);
};

class Edge
{
public:
	int v1,v2;
	int choice;
	double distance;
	Edge* next;
	Edge()
	{
		v1=v2=choice=0;
		distance=0;
	}
	Edge(const Edge&e)
	{
		v1=e.v1;
		v2=e.v2;
		choice=e.choice;
		distance=e.distance;
	}
	Edge& operator =( const Edge&);
};

class Triangle
{
public:
	int p[3];
	bool hasRemoved;
	Point n;
	double matrix[4][4];
public:
	Triangle()
	{
		p[0]=0;p[1]=0;p[2]=0;
		hasRemoved=false;
	}
};

Point operator + (const Point p,const Point q);
Point operator - (const Point p,const Point q);
Point operator * (const Point p,double factor);
Point operator / (const Point p,double factor);
bool operator !=(const Point a,const Point b);
bool operator ==(const Point a,const Point b);
Point XMul(const Point p,const Point q);
double Mul(const Point p,const Point q);
double p2pDis(const Point p,const Point q);

#endif
