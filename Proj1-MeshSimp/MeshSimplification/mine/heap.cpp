#include "heap.h"
#include<iostream>
using namespace std;
#define DefaultSize 4096

void Heap::siftUp(int start)
{
	int j = start;
	int i = (j-1)/2;
	Edge temp = heap[j];
	while (j > 0)
	{
		if (heap[i].distance < temp.distance)
			break;
		else
		{
			heap[j] = heap[i];
			j = i;
			i = (j-1)/2;
		}
	}
	heap[j] = temp;
}

void Heap::siftDown(int start, int m)
{
	int i=start;
	int j=2*i+1;
	Edge temp=heap[i];
	while(j<=m)
	{
		if((j<=m-1)&&heap[j].distance>heap[j+1].distance)
			j++;
		if(temp.distance<=heap[j].distance)
			break;
		else
		{
			heap[i]=heap[j];
			heap[i].v1=heap[j].v1;
			heap[i].v2=heap[j].v2;
			i=j;
			j=2*i+1;
		}
	}
	heap[i]=temp;
	heap[i].v1=temp.v1;
	heap[i].v2=temp.v2;
}

bool Heap::Insert(Edge edge)
{
	if(currentSize==0)
	{
		heap[currentSize]=edge;
		currentSize++;
		return true;
	}
	if(currentSize==maxSize)
	{
		maxSize+=DefaultSize;
		Edge* newHeap=new Edge[maxSize];
		for(int i=0;i<currentSize;++i)
			newHeap[i]=heap[i];
		delete []heap;
		heap=newHeap;

	}
	heap[currentSize]=edge;
	siftUp(currentSize);
	currentSize++;
	return true;
}

bool Heap::Remove(int index)
{
	if(index>=currentSize)
		return false;
	heap[index]=heap[currentSize-1];
	currentSize--;
	siftDown(index,currentSize-1);
	siftUp(index);
	return true;
}

bool Heap::RemoveMin(Edge &edge)
{
	if (currentSize==0)
		return false;
	edge=this->heap[0];
	heap[0]=heap[currentSize-1];
	currentSize--;
	siftDown(0,currentSize-1);
	return true;
}
