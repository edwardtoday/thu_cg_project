#include <stdio.h>
#include "paint.h"
#define WIDTH	800
#define HEIGHT	600

int main()
{
	drawing * raytracer;
	raytracer = new drawing(WIDTH,HEIGHT);
	raytracer->init();
	raytracer->clear(0);
	raytracer->render();

	bmpMap * bmp = new bmpMap();
	bmp->bmpHead.fileType = 19778;
	bmp->bmpHead.imageSize = 54+HEIGHT*WIDTH*3;
	bmp->bmpHead.blank = 0;
	bmp->bmpHead.startPosition = 54;

	bmp->infoHead.Length = 40;
	bmp->infoHead.width = WIDTH;
	bmp->infoHead.height = HEIGHT;
	bmp->infoHead.colorPlane = 1;
	bmp->infoHead.bitColor = 24;
	bmp->infoHead.zipFormat = 0;
	bmp->infoHead.realSize = HEIGHT*WIDTH*3;
	bmp->infoHead.xPels = 0;
	bmp->infoHead.yPels = 0;
	bmp->infoHead.colorUse = 0;
	bmp->infoHead.colorImportant = 0;
	bmp->bmpData = raytracer->getBuffer();

	writeBmp("result.bmp",bmp);
	return 0;
}
