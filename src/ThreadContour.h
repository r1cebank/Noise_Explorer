#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class ThreadContour : public ofThread
{
public:
	ThreadContour(ofxCvContourFinder*);
	~ThreadContour(void);
	void threadedFunction(void);

	//Var
	int *minArea, *maxArea, *nConsidered;
	bool *findHoles, *useApprox;
	ofxCvContourFinder *contourFinder;
	ofxCvGrayscaleImage grayImage;
};

