#include "ThreadContour.h"


ThreadContour::ThreadContour(ofxCvContourFinder *finder)
{
	contourFinder = finder;
}


ThreadContour::~ThreadContour(void)
{
}

void ThreadContour::threadedFunction(void)
{
	while(isThreadRunning()) {
		//lock();
		ofRectangle tempROI = grayImage.getROI();
		grayImage.resetROI();
		contourFinder->findContours(grayImage, *minArea, *maxArea, *nConsidered, *findHoles, *useApprox);
		grayImage.setROI(tempROI);
		//unlock();
	}
}