#include<iostream>
#include<cstdlib>
#include"materials.h"
#include"heap.h"
#include"mesh.h"
using namespace std;

int main(int argc, char* argv[])
{
	
	//mesh object("bunny.obj");
	mesh object(argv[1]);
	
	object.init();
	//double rate = 0.1;
	double rate = atof(argv[3]);
	//double rate;
	//cout<<"Please enter rate:";
	//cin>>rate;
	int total=object.vSize;
	for(int i=0;i<total*(1-rate);++i)
	{
		object.Delete();
		cout<<(double)i*100/total*(1-rate)<<"% has completed!\r";
	}
	object.storeObj(argv[2]);
	//object.storeObj("test.obj");
	
	return 0;
}
