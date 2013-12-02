#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"
#include "ofxDepthImageCompressor.h"
#include "DepthBucket.h"
#include "MeshLoader.h"

#define WIDTH 1200
#define HEIGHT 700
#define CONTROL_WIDTH 200
#define BANNER_HEIGHT 50

#define FILTER_MODE 1


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        //Misc Functions
        void setupUI();
        void exit();
        void guiEvent(ofxUIEventArgs &e);
        void countZero();
        void setValue();
		unsigned char getAverageFromImage(unsigned char* input, int size);
		unsigned char getPredictedValue(unsigned char* input, int size);
		void sharpenImage(bool state);
		void invertImage(bool state);
        void boxFilter();
		void drawContours();
		void setAvgDev(double &average, unsigned char &maxDev); 
    
        //UI
        ofxUICanvas *gui1, *gui2;
        ofxUILabel *title, *title2, *stat, *zeroFound, *selectedPixel, *pixelLocation, *pixelValue;
        ofxUIIntSlider *boundSlider, *sharpenBlurSlider, *minAreaSlider, *maxAreaSlider, *nConsideredSlider;
        ofxUIToggle *showFiltered, *sharpen, *showContour, *findHolesToggle, *useApproxToggle, *useInvert;
		//OpenCV
        ofxCvGrayscaleImage grayImage, previewImage, selectionImage, filteredImage, filterPreview, sharpenedImage, sharpenBackup;
		ofxCvGrayscaleImage contourImage;
		ofxCvGrayscaleImage invertBackup;
		ofxCvContourFinder contourFinder;
		//DepthImageCompressor
        ofxDepthImageCompressor compressor;
        //Misc Var
        ofImage decompressedImage;
        int zeroCounter;
        int selectedX, selectedY;
        int boundSize, sharpenBlurSize;
		int minArea, maxArea, nConsidered;
		bool findHoles, useApprox;
		unsigned char averageSelected;
        bool filterOn, sharpenOn, contourOn, invertOn;

		//Depth Bucket
		DepthBucket *bucket;

		//Mesh Loader
		MeshLoader *meshLoader;
};
