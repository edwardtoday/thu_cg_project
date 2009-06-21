#ifndef HEAP_H_
#define HEAP_H_

#include"materials.h"
#define DefaultSize 4096

class Heap
{
public:
	Edge* heap;
	int currentSize;
	int maxSize;
public:
	Heap()
	{
		maxSize=DefaultSize;
		heap=new Edge[maxSize];
		currentSize=0;
	}
	void siftDown(int start,int m);
	void siftUp(int start);
	bool Insert(Edge edge);
	bool Remove(int index);
	bool RemoveMin(Edge& edge);
};

#endif
