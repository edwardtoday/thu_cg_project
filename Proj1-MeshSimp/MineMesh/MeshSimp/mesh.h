#ifndef MESH_H_
#define MESH_H_

#include "materials.h"
#include "heap.h"
#include <string>
#include <fstream>
using namespace std;

class mesh
{
public:
	vector<Point> vertices;
	vector<Triangle> triangles;
	int vSize;
	int tSize;
	Heap* heap;

	mesh(string objName);
	double Distance(Point,Triangle);
	void setFn(int);
	void init();
	bool Delete();
	void storeObj(string objName);

	void calPointMatrix(int index);
	void calTriangleMatrix(int index);
	void calEdgeDistance(Edge&);
	bool isEqual(Triangle,Triangle);
	double calTheta(Edge);
	bool isIn(int,vector<int>);
};

#endif

