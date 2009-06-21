#include "bmpInfo.h"
#include<iostream>
#include<fstream>
#include<cmath>
#include<cstdlib>
using namespace std;

void readBmp(const char * src,bmpMap *bmp)					//读bmp文件
{
	ifstream fin;
	fin.open(src,ios::binary);
	if(!fin)
	{
		cout<<"输入文件名有误"<<endl;
		return;
	}
	
	fin.read((char*)(&(bmp->bmpHead.fileType)),sizeof(bmp->bmpHead.fileType));
	fin.read((char*)(&(bmp->bmpHead.imageSize)),sizeof(bmp->bmpHead.imageSize));
	fin.read((char*)(&(bmp->bmpHead.blank)),sizeof(bmp->bmpHead.blank));
	fin.read((char*)(&(bmp->bmpHead.startPosition)),sizeof(bmp->bmpHead.startPosition));

	fin.read((char*)(&(bmp->infoHead.Length)),sizeof(bmp->infoHead.Length));
	fin.read((char*)(&(bmp->infoHead.width)),sizeof(bmp->infoHead.width));
	fin.read((char*)(&(bmp->infoHead.height)),sizeof(bmp->infoHead.height));
	fin.read((char*)(&(bmp->infoHead.colorPlane)),sizeof(bmp->infoHead.colorPlane));
	fin.read((char*)(&(bmp->infoHead.bitColor)),sizeof(bmp->infoHead.bitColor));
	fin.read((char*)(&(bmp->infoHead.zipFormat)),sizeof(bmp->infoHead.zipFormat));
	fin.read((char*)(&(bmp->infoHead.realSize)),sizeof(bmp->infoHead.realSize));
	fin.read((char*)(&(bmp->infoHead.xPels)),sizeof(bmp->infoHead.xPels));
	fin.read((char*)(&(bmp->infoHead.yPels)),sizeof(bmp->infoHead.yPels));
	fin.read((char*)(&(bmp->infoHead.colorUse)),sizeof(bmp->infoHead.colorUse));
	fin.read((char*)(&(bmp->infoHead.colorImportant)),sizeof(bmp->infoHead.colorImportant));

	bmp->bmpData=new BmpData* [bmp->infoHead.height];
	int i,j;
	for(i=0;i<bmp->infoHead.height;i++)
	{
		bmp->bmpData[i]=new BmpData [bmp->infoHead.width];
	}
	for(i=0;i<bmp->infoHead.height;i++)
		for(j=0;j<bmp->infoHead.width;j++)
		{
			fin.read((char*)(&(bmp->bmpData[i][j].rgbBlue)),sizeof(bmp->bmpData[i][j].rgbBlue));
			fin.read((char*)(&(bmp->bmpData[i][j].rgbGreen)),sizeof(bmp->bmpData[i][j].rgbGreen));
			fin.read((char*)(&(bmp->bmpData[i][j].rgbRed)),sizeof(bmp->bmpData[i][j].rgbRed));
		}

	fin.close();
}

void writeBmp(const char * dst,bmpMap * bmp)			//写bmp文件
{
	ofstream fout;
	fout.open(dst,ios::binary);

	fout.write((char*)(&(bmp->bmpHead.fileType)),sizeof(bmp->bmpHead.fileType));
	fout.write((char*)(&(bmp->bmpHead.imageSize)),sizeof(bmp->bmpHead.imageSize));
	fout.write((char*)(&(bmp->bmpHead.blank)),sizeof(bmp->bmpHead.blank));
	fout.write((char*)(&(bmp->bmpHead.startPosition)),sizeof(bmp->bmpHead.startPosition));

	fout.write((char*)(&(bmp->infoHead.Length)),sizeof(bmp->infoHead.Length));
	fout.write((char*)(&(bmp->infoHead.width)),sizeof(bmp->infoHead.width));
	fout.write((char*)(&(bmp->infoHead.height)),sizeof(bmp->infoHead.height));
	fout.write((char*)(&(bmp->infoHead.colorPlane)),sizeof(bmp->infoHead.colorPlane));
	fout.write((char*)(&(bmp->infoHead.bitColor)),sizeof(bmp->infoHead.bitColor));
	fout.write((char*)(&(bmp->infoHead.zipFormat)),sizeof(bmp->infoHead.zipFormat));
	fout.write((char*)(&(bmp->infoHead.realSize)),sizeof(bmp->infoHead.realSize));
	fout.write((char*)(&(bmp->infoHead.xPels)),sizeof(bmp->infoHead.xPels));
	fout.write((char*)(&(bmp->infoHead.yPels)),sizeof(bmp->infoHead.yPels));
	fout.write((char*)(&(bmp->infoHead.colorUse)),sizeof(bmp->infoHead.colorUse));
	fout.write((char*)(&(bmp->infoHead.colorImportant)),sizeof(bmp->infoHead.colorImportant));

	int i,j;
	for(i=0;i<bmp->infoHead.height;i++)
		for(j=0;j<bmp->infoHead.width;j++)
		{
			fout.write((char*)(&(bmp->bmpData[i][j].rgbBlue)),sizeof(bmp->bmpData[i][j].rgbBlue));
			fout.write((char*)(&(bmp->bmpData[i][j].rgbGreen)),sizeof(bmp->bmpData[i][j].rgbGreen));
			fout.write((char*)(&(bmp->bmpData[i][j].rgbRed)),sizeof(bmp->bmpData[i][j].rgbRed));
		}

	fout.close();
}
