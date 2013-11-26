#pragma once
#include <iostream>
#include <sstream>
#include "ofxOpenCv.h"

using namespace std;

struct color{
	int r,g,b;
};

class DepthBucket
{
public:
	DepthBucket(void);
	DepthBucket(int,int);
	~DepthBucket(void);
	stringstream printBucket();
	int addDepthPixel(unsigned char, int);
	ofxCvColorImage getColoredImage();
	void done();
	bool getStatus();
private:
	int start, end;
	bool finish, finalized;
	int *bucket;
	color *colors;
	unsigned char *r;
	unsigned char *g;
	unsigned char *b;
	ofxCvGrayscaleImage red, green, blue;
	ofxCvColorImage colorImage;
	void init();
};

