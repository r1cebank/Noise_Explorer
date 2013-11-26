#include "DepthBucket.h"
#include "ofMain.h"

DepthBucket::DepthBucket(int s,int e)
{
	bucket = new int[e];
	colors = new color[e];
	r = new unsigned char[640*480];
	g = new unsigned char[640*480];
	b = new unsigned char[640*480];
	red.allocate(640, 480);
	green.allocate(640, 480);
	blue.allocate(640, 480);
	colorImage.allocate(640, 480);
	start = s;
	end = e;
	init();
}

int DepthBucket::addDepthPixel(unsigned char pixel, int pixelPosition)
{
	int intValue = (int)pixel;
	if(intValue < start && intValue < end)
		return -1;
	bucket[intValue]++;
	//Setting Colors
	r[pixelPosition] = (unsigned char) colors[intValue].r;
	g[pixelPosition] = (unsigned char) colors[intValue].g;
	b[pixelPosition] = (unsigned char) colors[intValue].b;
}

stringstream DepthBucket::printBucket()
{
	finish = true;
	stringstream str;
	cout << "<Printing Bucket>" << endl;
	for (int i = start; i < end; i++)
	{
		cout << "<" << i << ">" << ": " << bucket[i] << endl; 
	}
	return str;
}
void DepthBucket::init()
{
	finish = false;
	finalized = false;
	for(int i = 0; i < end; i++)
		bucket[i] = 0;
	for(int i = 0; i < end; i++)
	{
		colors[i].r = ofRandom(i, i + 10);
		colors[i].g = ofRandom(i - 10, i);
		colors[i].b = ofRandom(i, 255);
	}
}

void DepthBucket::done()
{
	finish = true;
}

bool DepthBucket::getStatus()
{
	return finish;
}

ofxCvColorImage DepthBucket::getColoredImage()
{
	if(!finalized)
	{
		red.setFromPixels(r, 640, 480);
		green.setFromPixels(g, 640, 480);
		blue.setFromPixels(b, 640, 480);
		colorImage.setFromGrayscalePlanarImages(red, green, blue);
		finalized = true;
		return colorImage;
	}
	return colorImage;
}
DepthBucket::DepthBucket(void)
{
	DepthBucket(0, 255);
}


DepthBucket::~DepthBucket(void)
{
}
