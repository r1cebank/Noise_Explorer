#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"
#include "ofxDepthImageCompressor.h"

#define WIDTH 1200
#define HEIGHT 700
#define CONTROL_WIDTH 200
#define BANNER_HEIGHT 50

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
    
        //UI
        ofxUICanvas *gui1, *gui2;
        ofxUILabel *title, *title2, *stat, *zeroFound, *selectedPixel, *pixelLocation, *pixelValue;
        ofxUIIntSlider *slider;
		//OpenCV
        ofxCvGrayscaleImage grayImage, previewImage, selectionImage;
        //DepthImageCompressor
        ofxDepthImageCompressor compressor;
        //Misc Var
        ofImage decompressedImage;
        int zeroCounter;
        int selectedX, selectedY;
        int boundSize;
		unsigned char averageSelected;
};
