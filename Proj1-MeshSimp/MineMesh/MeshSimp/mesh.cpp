#include "mesh.h"
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<cmath>
using namespace std;

mesh::mesh(string objName)
{
	this->tSize=0;
	this->vSize=0;
	ifstream fin;
	fin.open(objName.c_str());
	string temp;
	Point point;
	point.x=point.y=point.z=0;
	point.index=0;
	point.hasRemoved=true;
	vertices.push_back(point);
	Triangle tri;
	tri.p[0]=0;tri.p[1]=0;tri.p[2]=0;
	tri.hasRemoved=true;
	triangles.push_back(tri);
	int count=1;
	int count2=1;
	while(getline(fin,temp))
	{
		if(temp[0]!='#')
		{
			if(temp[0]=='v')
			{
				temp=temp.substr(2);
				int p=(int)temp.find(' ',0);
				string sx=temp.substr(0,p);
				double x=atof(sx.c_str());
				temp=temp.substr(p+1);
				p=(int)temp.find(' ',0);
				string sy=temp.substr(0,p);
				double y=atof(sy.c_str());
				temp=temp.substr(p+1);
				p=(int)temp.find(' ',0);
				string sz=temp.substr(0,p);
				double z=atof(sz.c_str());
				Point point;
				point.x=x;point.y=y;point.z=z;
				point.index=count;
				point.hasRemoved=false;
				vertices.push_back(point);
				count++;
			}
			else if(temp[0]=='f')
			{
				temp=temp.substr(2);
				int p=(int)temp.find(' ',0);
				string sx=temp.substr(0,p);
				int x=atoi(sx.c_str());
				temp=temp.substr(p+1);
				p=(int)temp.find(' ',0);
				string sy=temp.substr(0,p);
				int y=atoi(sy.c_str());
				temp=temp.substr(p+1);
				p=(int)temp.find(' ',0);
				string sz=temp.substr(0,p);
				int z=atoi(sz.c_str());	
				Triangle tri;
				tri.p[0]=x;tri.p[1]=y;tri.p[2]=z;
				tri.hasRemoved=false;
				(vertices[tri.p[0]]).tris.push_back(count2);
				(vertices[tri.p[1]]).tris.push_back(count2);
				(vertices[tri.p[2]]).tris.push_back(count2);
				triangles.push_back(tri);
				count2++;
			}
		}
	}
	fin.close();
	this->vSize=count-1;
	this->tSize=count2-1;
	this->heap=new Heap();
}

double mesh::Distance(Point,Triangle)
{
	return 0;
}

bool mesh::Delete()
{
	if(heap->currentSize==0)
		return false;
	Edge minEdge;
	heap->RemoveMin(minEdge);
	Point p1=vertices[minEdge.v1];
	Point p2=vertices[minEdge.v2];
	int v1=minEdge.v1;
	int v2=minEdge.v2;
	vector<int> node1;
	vector<int> node2;
	for(int i=0;i<(int)p1.tris.size();++i)
	{
		if(!triangles[p1.tris[i]].hasRemoved)
		{
			if(triangles[p1.tris[i]].p[0]!=v1&&triangles[p1.tris[i]].p[0]!=v2)
			{
				if(!isIn(triangles[p1.tris[i]].p[0],node1))
					node1.push_back(triangles[p1.tris[i]].p[0]);
			}
			if(triangles[p1.tris[i]].p[1]!=v1&&triangles[p1.tris[i]].p[1]!=v2)
			{
				if(!isIn(triangles[p1.tris[i]].p[1],node1))
					node1.push_back(triangles[p1.tris[i]].p[1]);
			}
			if(triangles[p1.tris[i]].p[2]!=v1&&triangles[p1.tris[i]].p[2]!=v2)
			{
				if(!isIn(triangles[p1.tris[i]].p[2],node1))
					node1.push_back(triangles[p1.tris[i]].p[2]);
			}
		}
	}
	for(int i=0;i<(int)p2.tris.size();++i)
	{
		if(!triangles[p2.tris[i]].hasRemoved)
		{
			if(triangles[p2.tris[i]].p[0]!=v1&&triangles[p2.tris[i]].p[0]!=v2)
			{
				if(!isIn(triangles[p2.tris[i]].p[0],node2))
					node2.push_back(triangles[p2.tris[i]].p[0]);
			}
			if(triangles[p2.tris[i]].p[1]!=v1&&triangles[p2.tris[i]].p[1]!=v2)
			{
				if(!isIn(triangles[p2.tris[i]].p[1],node2))
					node2.push_back(triangles[p2.tris[i]].p[1]);
			}
			if(triangles[p2.tris[i]].p[2]!=v1&&triangles[p2.tris[i]].p[2]!=v2)
			{
				if(!isIn(triangles[p2.tris[i]].p[2],node2))
					node2.push_back(triangles[p2.tris[i]].p[2]);
			}
		}
	}
	int* nodeNum=new int[2];
	int ct=0;
	for(int i=0;i<(int)node1.size();++i)
	{
		for(int j=0;j<(int)node2.size();++j)
		{
			if(node1[i]==node2[j])
			{
				nodeNum[ct]=node1[i];
				ct++;
				if(ct==2) break;
			}
		}
	}
	Point newPoint=(vertices[minEdge.v1]+vertices[minEdge.v2])/2;
	newPoint.index=(int)vertices.size();
	vertices.push_back(newPoint);

	vertices[minEdge.v1].hasRemoved=true;
	vertices[minEdge.v2].hasRemoved=true;
	this->vSize--;

	for(int i=0;i<(int)vertices[minEdge.v1].tris.size();++i)
	{
		int num=vertices[minEdge.v1].tris[i];
		if(triangles[num].p[0]==minEdge.v2||triangles[num].p[2]==minEdge.v2||triangles[num].p[2]==minEdge.v2)
		{
			triangles[num].hasRemoved=true;
		}
		else
		{
			if(triangles[num].p[0]==minEdge.v1)
			{
				triangles[num].p[0]=newPoint.index;
				vertices[vertices.size()-1].tris.push_back(num);
			}
			else if(triangles[num].p[1]==minEdge.v1)
			{
				triangles[num].p[1]=newPoint.index;
				vertices[vertices.size()-1].tris.push_back(num);
			}
			else if(triangles[num].p[2]==minEdge.v1)
			{
				triangles[num].p[2]=newPoint.index;
				vertices[vertices.size()-1].tris.push_back(num);
			}
			calTriangleMatrix(num);
		}
	}

	for(int i=0;i<(int)vertices[minEdge.v2].tris.size();++i)
	{
		int num=vertices[minEdge.v2].tris[i];
		if(triangles[num].p[0]==minEdge.v1||triangles[num].p[2]==minEdge.v1||triangles[num].p[2]==minEdge.v1)
		{}
		else
		{
			if(triangles[num].p[0]==minEdge.v2)
			{
				triangles[num].p[0]=newPoint.index;
				vertices[vertices.size()-1].tris.push_back(num);
			}
			else if(triangles[num].p[1]==minEdge.v2)
			{
				triangles[num].p[1]=newPoint.index;
				vertices[vertices.size()-1].tris.push_back(num);
			}
			else if(triangles[num].p[2]==minEdge.v2)
			{
				triangles[num].p[2]=newPoint.index;
				vertices[vertices.size()-1].tris.push_back(num);
			}
			calTriangleMatrix(num);
		}
	}

	for(int i=0;i<(int)vertices[vertices.size()-1].tris.size();++i)
	{
		int n=vertices[vertices.size()-1].tris[i];
		if(!triangles[n].hasRemoved)
		{
			if(triangles[n].p[0]!=newPoint.index)
				calPointMatrix(triangles[n].p[0]);
			if(triangles[n].p[1]!=newPoint.index)
				calPointMatrix(triangles[n].p[1]);
			if(triangles[n].p[2]!=newPoint.index)
				calPointMatrix(triangles[n].p[2]);
		}
	}
	calPointMatrix(newPoint.index);

	vector<Edge> changes;
	bool sign1=true;
	bool sign2=true;
	Edge* newHeap=new Edge[heap->currentSize];
	int count=0;
	for(int i=0;i<(int)heap->currentSize;++i)
	{
		if(heap->heap[i].v1==v1||heap->heap[i].v1==v2)
		{
			if(heap->heap[i].v2==nodeNum[0]&&sign1)
			{
				sign1=false;
			}
			else if(heap->heap[i].v2==nodeNum[1]&&sign2)
			{
				sign2=false;
			}
			else
			{
				heap->heap[i].v1=newPoint.index;
				calEdgeDistance(heap->heap[i]);
				changes.push_back(heap->heap[i]);
			}
		}
		else if(heap->heap[i].v2==v1||heap->heap[i].v2==v2)
		{
			if(heap->heap[i].v1==nodeNum[0]&&sign1)
			{
				sign1=false;
			}
			else if(heap->heap[i].v1==nodeNum[1]&&sign2)
			{
				sign2=false;
			}
			else
			{
				heap->heap[i].v2=newPoint.index;
				calEdgeDistance(heap->heap[i]);
				changes.push_back(heap->heap[i]);
			}
		}
		else
		{
			newHeap[count]=heap->heap[i];
			count++;
		}
	}
	delete []heap->heap;
	heap->heap=newHeap;
	heap->currentSize=count;
	int current=(count-2)/2;
	while(current>=0)
	{
		heap->siftDown(current,heap->currentSize-1);
		current--;
	}
	for(int i=0;i<(int)changes.size();++i)
	{
		heap->Insert(changes[i]);
	}
	return true;
}

void mesh::setFn(int index)
{
	Point p1=vertices[triangles[index].p[1]]-vertices[triangles[index].p[0]];
	Point p2=vertices[triangles[index].p[2]]-vertices[triangles[index].p[0]];
	triangles[index].n=XMul(p1,p2);
}

void mesh::init()
{
	cout<<"Load OBJ ok!"<<endl;
	cout<<"vertices:"<<this->vSize<<endl;
	cout<<"triangles:"<<this->tSize<<endl;
	for(int i=1;i<(int)triangles.size();++i)
		calTriangleMatrix(i);
	for(int i=1;i<(int)vertices.size();++i)
		calPointMatrix(i);
	vector<Edge> edges;
	for(int i=1;i<(int)triangles.size();++i)
	{
		Edge e1,e2,e3;
		e1.v1=triangles[i].p[0];
		e1.v2=triangles[i].p[1];
		e2.v1=triangles[i].p[1];
		e2.v2=triangles[i].p[2];
		e3.v1=triangles[i].p[2];
		e3.v2=triangles[i].p[0];
		calEdgeDistance(e1);
		calEdgeDistance(e2);
		calEdgeDistance(e3);			
		if(e1.v1>e1.v2) edges.push_back(e1);
		if(e2.v1>e2.v2) edges.push_back(e2);
		if(e3.v1>e3.v2) edges.push_back(e3);
	}
	for(int i=0;i<(int)edges.size();++i)
	{
		heap->Insert(edges[i]);
	}
	cout<<"size="<<edges.size()<<endl;
}

void mesh::calPointMatrix(int index)
{
	for(int k1=0;k1<4;++k1)
		for(int k2=0;k2<4;++k2)
			vertices[index].matrix[k1][k2]=0;
	for(int i=0;i<(int)vertices[index].tris.size();++i)
	{
		if(!triangles[vertices[index].tris[i]].hasRemoved)
		{
			for(int k1=0;k1<4;++k1)
				for(int k2=0;k2<4;++k2)
					vertices[index].matrix[k1][k2]+=triangles[vertices[index].tris[i]].matrix[k1][k2];
		}
	}
}

void mesh::calTriangleMatrix(int index)
{
	Point p0=vertices[triangles[index].p[0]];
	Point p1=vertices[triangles[index].p[1]];
	Point p2=vertices[triangles[index].p[2]];
	Point u,v;
	u=p1-p0;
	v=p2-p0;
	double a1=u.y*v.z-v.y*u.z;
	double b1=u.z*v.x-v.z*u.x;
	double c1=u.x*v.y-v.x*u.y;
	double t=sqrt(a1*a1+b1*b1+c1*c1);
	double a,b,c,d;
	if(t==0)
	{
		a=b=c=d=0;
	}
	else
	{
		a=a1/t;
		b=b1/t;
		c=c1/t;
		d=-p0.x*a-p0.y*b-p0.z*c;
	}

	triangles[index].matrix[0][0]=a*a;
	triangles[index].matrix[0][1]=triangles[index].matrix[1][0]=a*b;
	triangles[index].matrix[0][2]=triangles[index].matrix[2][0]=a*c;
	triangles[index].matrix[0][3]=triangles[index].matrix[3][0]=a*d;
	triangles[index].matrix[1][1]=b*b;
	triangles[index].matrix[1][2]=triangles[index].matrix[2][1]=b*c;
	triangles[index].matrix[1][3]=triangles[index].matrix[3][1]=b*d;
	triangles[index].matrix[2][2]=c*c;
	triangles[index].matrix[2][3]=triangles[index].matrix[3][2]=c*d;
	triangles[index].matrix[3][3]=d*d;
}

void mesh::calEdgeDistance(Edge& e)
{
	Point p1=vertices[e.v1];
	Point p2=vertices[e.v2];
	Point m=(p1+p2)/2;

	double matrix[4][4];
	for(int i=0;i<4;++i)
	{
		for(int j=0;j<4;++j)
			matrix[i][j]=p1.matrix[i][j]+p2.matrix[i][j];
	}
	double a0=m.x*matrix[0][0]+m.y*matrix[1][0]+m.z*matrix[2][0]+matrix[3][0];
	double a1=m.x*matrix[0][1]+m.y*matrix[1][1]+m.z*matrix[2][1]+matrix[3][1];
	double a2=m.x*matrix[0][2]+m.y*matrix[1][2]+m.z*matrix[2][2]+matrix[3][2];
	double a3=m.x*matrix[0][3]+m.y*matrix[1][3]+m.z*matrix[2][3]+matrix[3][3];
	e.distance=a0*m.x+a1*m.y+a2*m.z+a3;
}

void mesh::storeObj(string objName)
{
	ofstream fout;
	fout.open(objName.c_str());
	vector<Point> vtx;
	vector<int> index;
	vector<Triangle> tri;
	for(int i=0;i<(int)vertices.size();++i)
	{
		if(vertices[i].hasRemoved)
			index.push_back(0);
		else
		{
			vtx.push_back(vertices[i]);
			index.push_back((int)vtx.size());
		}
	}
	for(int i=0;i<(int)triangles.size();++i)
	{
		if(!triangles[i].hasRemoved)
			tri.push_back(triangles[i]);
	}
	fout<<"# "<<vtx.size()<<" vertices"<<endl;
	for(int i=0;i<(int)vtx.size();++i)
		fout<<"v "<<vtx[i].x<<" "<<vtx[i].y<<" "<<vtx[i].z<<endl;
	fout<<"# "<<tri.size()<<" triangles"<<endl;
	for(int j=0;j<(int)tri.size();++j)
		fout<<"f "<<index[tri[j].p[0]]<<" "<<index[tri[j].p[1]]<<" "<<index[tri[j].p[2]]<<endl;
	fout.close();
}

bool mesh::isEqual(Triangle p,Triangle q)
{
	if(p.p[0]==q.p[0]&&p.p[1]==q.p[1]&&p.p[2]==q.p[2])
		return true;
	if(p.p[0]==q.p[0]&&p.p[2]==q.p[1]&&p.p[1]==q.p[2])
		return true;
	if(p.p[1]==q.p[0]&&p.p[2]==q.p[1]&&p.p[0]==q.p[2])
		return true;
	if(p.p[1]==q.p[0]&&p.p[0]==q.p[1]&&p.p[2]==q.p[2])
		return true;
	if(p.p[2]==q.p[0]&&p.p[0]==q.p[1]&&p.p[1]==q.p[2])
		return true;
	if(p.p[2]==q.p[0]&&p.p[1]==q.p[1]&&p.p[0]==q.p[2])
		return true;
	return false;
}

double mesh::calTheta(Edge e)
{
	int v1=e.v1;
	int v2=e.v2;
	int m[2];
	int count=0;
	for(int i=0;i<(int)vertices[v1].tris.size();++i)
	{
		if(triangles[vertices[v1].tris[i]].p[0]==v2||triangles[vertices[v1].tris[i]].p[1]==v2||triangles[vertices[v1].tris[i]].p[2]==v2)
		{
			m[count]=vertices[v1].tris[i];
			count++;
			if(count==2) break;
		}
	}
	if(isEqual(triangles[m[0]],triangles[m[1]])) return 1;
	this->setFn(m[1]);
	double result=Mul(triangles[m[0]].n,triangles[m[1]].n);
	return result;
}

bool mesh::isIn(int index, vector<int> num)
{
	bool sign=false;
	for(int i=0;i<(int)num.size();++i)
	{
		if(index==num[i])
		{
			sign=true;
			break;
		}
	}
	return sign;
}
