#include <cstdio>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "paint.h"
using namespace std;

#define FRAME_NUM 1+10
int main()
{
	const int frameW = 640;
	const int frameH = 480;

	IplImage* pImg = 0;
	cvNamedWindow( "Image", 1 );//创建窗口
	cvResizeWindow("Image",frameW,frameH);

	for(int i = 1; i < FRAME_NUM; i++)
	{
		cout<<"Rendering frame "<<i<<". Please wait...";

		drawing * raytracer;
		raytracer = new drawing(frameW,frameH);
		raytracer->init(i);
		raytracer->clear(0);
		raytracer->render();

		bmpMap * bmp = new bmpMap();
		bmp->bmpHead.fileType = 19778;
		bmp->bmpHead.imageSize = 54+frameH*frameW*3;
		bmp->bmpHead.blank = 0;
		bmp->bmpHead.startPosition = 54;

		bmp->infoHead.Length = 40;
		bmp->infoHead.width = frameW;
		bmp->infoHead.height = frameH;
		bmp->infoHead.colorPlane = 1;
		bmp->infoHead.bitColor = 24;
		bmp->infoHead.zipFormat = 0;
		bmp->infoHead.realSize = frameH*frameW*3;
		bmp->infoHead.xPels = 0;
		bmp->infoHead.yPels = 0;
		bmp->infoHead.colorUse = 0;
		bmp->infoHead.colorImportant = 0;
		bmp->bmpData = raytracer->getBuffer();

		char outputname[30] = "frame.bmp";
		//sprintf(outputname,"Frame%d.bmp",i);
		cout<<"\tSaving "<<outputname<<"..."<<endl;

		writeBmp(outputname,bmp);
		pImg = cvLoadImage(outputname);
		cvShowImage( "Image", pImg );//显示图像
		cvWaitKey(20);
	}

	//system("PAUSE");
	cvDestroyWindow( "Image" );//销毁窗口
	cvReleaseImage( &pImg ); //释放图像

	return 0;
}
