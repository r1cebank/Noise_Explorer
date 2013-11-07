/*
 *  ofxDepthImageCompressor.h
 *  ofxRGBDepthCaptureOpenNI
 *
 *  Created by James George on 12/20/11.
 *
 */

#pragma once

#include "ofMain.h"

class ofxDepthImageCompressor {
  public:
	ofxDepthImageCompressor();
	~ofxDepthImageCompressor();
	
	//COMPRESS
	bool saveToRaw(string filename, unsigned short* buf);
	void saveToCompressedPng(string filename, unsigned short* buf);
	
	//DECOMPRESS
	static void readCompressedPng(string filename, ofShortPixels& pix);
	unsigned short* readDepthFrame(string filename, unsigned short* outbuf = NULL);
	unsigned short* readDepthFrame(ofFile& file, unsigned short*  outbuf = NULL);
	static unsigned short* readCompressedPng(string filename, unsigned short* outbuf = NULL);
	
	ofImage readDepthFrametoImage(string filename);
	
	ofImage convertTo8BitImage(ofShortPixels& pix, bool createTexture = true);
	ofImage convertTo8BitImage(unsigned short* buf, bool ceateTexture = true);
	
	void convertTo8BitImage(ofShortPixels& pix, ofImage& image);
	void convertTo8BitImage(unsigned short* buf, ofImage& image);
	
  protected:
	ofImage compressedDepthImage;

};
