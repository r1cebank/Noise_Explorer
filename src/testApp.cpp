#include "testApp.h"

/*
	image pipeline
	read image -> decompressedImage & grayImage (copy)
	if filter on -> grayImage -> filter
	if sharpen on -> grayImage -> sharpen

*/

//--------------------------------------------------------------
void testApp::setup(){
	ofSeedRandom(ofGetElapsedTimef());
	bucket = new DepthBucket(1, 255);
    ofSetBackgroundColor(179, 242, 255);
    setupUI();
	ofSetLogLevel(OF_LOG_VERBOSE);
    decompressedImage = compressor.readDepthFrametoImage("capture/1.raw");
    grayImage.allocate(640, 480);
    previewImage.allocate(1,1);
    selectionImage.allocate(400,400);
	filterPreview.allocate(boundSize*2+1, boundSize*2+1);
	sharpenedImage.allocate(640,480);
    grayImage.setFromPixels(decompressedImage.getPixelsRef());
    filteredImage.setFromPixels(decompressedImage.getPixelsRef());
	sharpenedImage.setFromPixels(decompressedImage.getPixelsRef());
	countZero();
    zeroFound->setLabel(ofToString(zeroCounter) + " zeros found.");
    selectedX = selectedY = 100;
    /*filterOn = sharpenOn = contourOn = findHoles = useApprox = false;
	minArea = maxArea = nConsidered = 0;*/
}

void testApp::setupUI() {
    gui1 = new ofxUICanvas(0,0, WIDTH, BANNER_HEIGHT);
    gui2 = new ofxUICanvas(WIDTH-CONTROL_WIDTH, BANNER_HEIGHT, CONTROL_WIDTH, HEIGHT);
    title = new ofxUILabel("Noise Explorer by Siyuan Gao", OFX_UI_FONT_MEDIUM);
    title2 = new ofxUILabel("Tohoku University, Purdue University", OFX_UI_FONT_SMALL);
    gui1->addWidgetDown(title);
    gui1->addWidgetDown(title2);
    stat = new ofxUILabel("Stat", OFX_UI_FONT_MEDIUM);
    zeroFound = new ofxUILabel("Zero Found", OFX_UI_FONT_SMALL);
    selectedPixel = new ofxUILabel("Selected Pixel", OFX_UI_FONT_MEDIUM);
    pixelLocation = new ofxUILabel("Pixel Location", OFX_UI_FONT_SMALL);
    pixelValue = new ofxUILabel("Pixel Value", OFX_UI_FONT_SMALL);
    showFiltered = new ofxUIToggle("Show Filtered", &filterOn, 15, 15);
	sharpen = new ofxUIToggle("Sharpen", &sharpenOn, 15, 15);
    gui2->addWidgetDown(stat);
    gui2->addSpacer();
    gui2->addWidgetDown(zeroFound);
    gui2->addWidgetDown(selectedPixel);
    gui2->addSpacer();
    gui2->addWidgetDown(pixelLocation);
    gui2->addWidgetDown(pixelValue);
    gui2->addSpacer();
    boundSlider = new ofxUIIntSlider("Bound Size", 1, 40, &boundSize, 170, 20);
	sharpenBlurSlider = new ofxUIIntSlider("Sharpen Blur", 1, 40, &sharpenBlurSize, 170, 20);
	showContour = new ofxUIToggle("Show Contour", &contourOn, 15, 15);
    gui2->addWidgetDown(showFiltered);
	gui2->addWidgetDown(boundSlider);
	gui2->addWidgetDown(sharpen);
	gui2->addWidgetDown(sharpenBlurSlider);
	//Contour Finder Here
	gui2->addWidgetDown(showContour);
	minAreaSlider = new ofxUIIntSlider("Min Area", 1, 640*480, &minArea, 170, 20);
	maxAreaSlider = new ofxUIIntSlider("Max Area", 1, 640*480, &maxArea, 170, 20);
	nConsideredSlider = new ofxUIIntSlider("nConsidered", 1, 100, &nConsidered, 170, 20);
	findHolesToggle = new ofxUIToggle("Find Holes", &findHoles, 15, 15);
	useApproxToggle = new ofxUIToggle("Use Approximation", &useApprox, 15, 15);
	gui2->addWidgetDown(minAreaSlider);
	gui2->addWidgetDown(maxAreaSlider);
	gui2->addWidgetDown(nConsideredSlider);
	gui2->addWidgetDown(findHolesToggle);
	gui2->addWidgetDown(useApproxToggle);
	gui2->addSpacer();
	useInvert = new ofxUIToggle("Invert", &invertOn, 15, 15);
	gui2->addWidgetDown(useInvert);
	gui2->addFPS();
	gui2->loadSettings("settings.xml");
	filterOn = sharpenOn = contourOn = invertOn = false;
	ofAddListener(gui2->newGUIEvent, this, &testApp::guiEvent);
}

void testApp::countZero() {
	zeroCounter = 0;
    for(int i = 0; i< 640*480; i++) {
		bucket->addDepthPixel(grayImage.getPixels()[i], i);
        if(grayImage.getPixels()[i] == 0) {
			zeroCounter++;
		}
    }
	bucket->printBucket();
}

//--------------------------------------------------------------
void testApp::update(){
    setValue();
	if(contourOn){
		//ofLogNotice() << "Finding Contours";
		ofRectangle tempROI = filteredImage.getROI();
		filteredImage.resetROI();
		contourFinder.findContours(filteredImage, minArea, maxArea, nConsidered, findHoles, useApprox);
		filteredImage.setROI(tempROI);
	}
}

void testApp::drawContours(){
	float x, y, area, length, width, height;
	float realX, realY;
	ofPoint centroid;
	ofxCvBlob blob;
	ofNoFill();
	if(contourOn){
		contourFinder.draw(20, 70, 640, 480);
		ofColor c(255, 0, 0);
		for(int i = 0; i < contourFinder.nBlobs; i++) {
			//Setting useful variables
			blob = contourFinder.blobs.at(i);
			ofRectangle r = blob.boundingRect;
			x = r.x; y = r.y; width = r.width; height = r.height; centroid = blob.centroid; area = blob.area;
			length = blob.length; //Messy no return multiline

			//Drawing rectangle
			r.x += 20; r.y += 70;
			c.setHsb(i * 64, 255, 255);
			ofSetColor(c);
			ofRect(r);
			//Drawing point
			ofCircle(centroid.x + 20, centroid.y + 70, 4);

			//Drawing informations
			realX = x + 20;
			realY = y + 70;
			ofDrawBitmapString("x:" + ofToString(x) + " y:" + ofToString(y), realX, realY - 11);
			ofDrawBitmapString("width:" + ofToString(width) + " height:" + ofToString(height), realX, realY - 11*2);
			ofDrawBitmapString("area:" + ofToString(area) + " length:" + ofToString(length), realX, realY - 11*3);
		}
	}
	ofFill();
	ofSetColor(ofColor::white);
}

void testApp::boxFilter(){
    filteredImage.setFromPixels(grayImage.getPixelsRef());
    unsigned char *original = grayImage.getPixels();
    unsigned char *filtered = filteredImage.getPixels();
    int filterX = 0, filterY = 0;
	long long start = ofGetElapsedTimeMillis();
    for(int i = 0; i < 640*480; i++){
        if(original[i] == 0){
            //Set ROI, get roi image, get average and update
            //filtered[i] = 0;
            filterY = i / 640;
            filterX = i % 640;
            filteredImage.resetROI();
            filteredImage.setROI(filterX - boundSize, filterY - boundSize, boundSize*2+1, boundSize*2+1);
			averageSelected = getAverageFromImage(filteredImage.getRoiPixels(), pow((double)((boundSize * 2)+ 1), 2));
			filtered[i] = averageSelected;
        }
            //Find the zeros in image and replace with the average.
    }
	long long end = ofGetElapsedTimeMillis();
	ofLogNotice() << "Elapsed time: " << end-start << "ms";
}

void testApp::setValue() {
    if(filterOn){
        pixelLocation->setLabel("(" + ofToString(selectedX) + ", " + ofToString(selectedY) + ")");
        pixelValue->setLabel("Value: " + ofToString((int)filteredImage.getPixels()[selectedY * 640 + selectedX]));
        filteredImage.setROI(selectedX, selectedY, 1, 1);
        previewImage.setFromPixels(filteredImage.getRoiPixelsRef());
        filteredImage.resetROI();
        filteredImage.setROI(selectedX - boundSize, selectedY - boundSize, boundSize*2+1, boundSize*2+1);
        selectionImage.setFromPixels(filteredImage.getRoiPixelsRef());
        averageSelected = getAverageFromImage(selectionImage.getPixels(), pow((double)((boundSize * 2)+ 1), 2));
    }
    else{
        pixelLocation->setLabel("(" + ofToString(selectedX) + ", " + ofToString(selectedY) + ")");
        pixelValue->setLabel("Value: " + ofToString((int)grayImage.getPixels()[selectedY * 640 + selectedX]));
        grayImage.setROI(selectedX, selectedY, 1, 1);
        previewImage.setFromPixels(grayImage.getRoiPixelsRef());
        grayImage.resetROI();
        grayImage.setROI(selectedX - boundSize, selectedY - boundSize, boundSize*2+1, boundSize*2+1);
        selectionImage.setFromPixels(grayImage.getRoiPixelsRef());
        averageSelected = getAverageFromImage(selectionImage.getPixels(), pow((double)((boundSize * 2)+ 1), 2));
    }
}

unsigned char testApp::getAverageFromImage(unsigned char* input, int size){
	int totalValue = 0, nonZero = 0;
	for(int i = 0; i < size; i++){
		if(input[i] != 0){
				totalValue += input[i];
				nonZero++;
				break;
		}
	}
	if(nonZero != 0)
		return (unsigned char)(totalValue / nonZero);
	else
		return 0;
}

unsigned char testApp::getPredictedValue(unsigned char* input, int size){
	int totalValue = 0, nonZero = 0;
	for(int i = 0; i < size; i++){
		if(input[i] != 0){
			totalValue += input[i];
			nonZero++;
		}
	}
	if(nonZero != 0)
		return (unsigned char)(totalValue / nonZero);
	else
		return 0;
}

//--------------------------------------------------------------
void testApp::draw(){
    if(!filterOn && !sharpenOn){
        grayImage.draw(20, 70);
	}
    else
        filteredImage.draw(20, 70);
	if(bucket->getStatus())
		bucket->getColoredImage().draw(20, 70);
	if(contourOn)
		drawContours();
    ofDrawBitmapStringHighlight("Preview", 680, 80, ofColor::seaGreen, ofColor::white);
	ofDrawBitmapStringHighlight("Average Selected: " + ofToString((int)averageSelected), 680, 550, ofColor::seaGreen, ofColor::white);
    previewImage.draw(680, 90, 100, 100);
    ofDrawBitmapStringHighlight(pixelValue->getLabel(), 680, 210, ofColor::seaGreen, ofColor::white);
    selectionImage.draw(680, 220, 301, 301);
    ofDrawBitmapStringHighlight("x: " + ofToString(mouseX) + " y: " + ofToString(mouseY), 20, 680, ofColor::seaGreen, ofColor::white);
    ofSetColor(ofColor::red);
    ofRect(selectedX + 20, selectedY + 70, 1, 1);
	ofNoFill();
    ofRect(680, 90, 100, 100);
    ofLine(selectedX + 20, selectedY + 70, selectedX + 40 + boundSize, selectedY + 40 - boundSize);
    ofDrawBitmapString("candidate", selectedX + 40 + boundSize, selectedY + 40 - boundSize);
    ofSetColor(ofColor::green);
    ofRect(selectedX + 20 - boundSize, selectedY + 70 - boundSize, boundSize*2+1, boundSize*2+1);
    ofLine(selectedX - boundSize + 20, selectedY + boundSize + 70, selectedX - boundSize, selectedY + boundSize + 100);
    ofDrawBitmapString("bounded pixels", selectedX - boundSize - 120, selectedY + boundSize + 110);
    ofSetColor(ofColor::white);
	ofFill();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofLogNotice() << "Key pressed " << key;
    switch(key){
        case 357:
            ofLogNotice() << "moving pixel up";
            selectedY--;
            if(selectedY < 0)
                selectedY = 0;
            break;
        case 359:
            ofLogNotice() << "moving pixel down";
            selectedY++;
            if(selectedY > 479)
                selectedY = 479;
            break;
        case 356:
            ofLogNotice() << "moving pixel left";
            selectedX--;
            if(selectedX < 0)
                selectedX = 0;
            break;
        case 358:
            ofLogNotice() << "moving pixel right";
            selectedX++;
            if(selectedX > 639)
                selectedX = 639;
            break;
    }
    if(key >= 356 && key <= 359)
        setValue();
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if((mouseX >= 20 && mouseY >= 70) && (mouseX < 20 + 640 && mouseY < 70 + 480)) {
        //ofLogNotice() << "Mouse Selected at Pixel";
        selectedX = mouseX - 20;
        selectedY = mouseY - 70;
        setValue();
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if((mouseX >= 20 && mouseY >= 70) && (mouseX < 20 + 640 && mouseY < 70 + 480)) {
        //ofLogNotice() << "Mouse Selected at Pixel";
        selectedX = mouseX - 20;
        selectedY = mouseY - 70;
        setValue();
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::exit(){
	gui2->saveSettings("settings.xml");
    delete gui1;
	delete gui2;
}

void testApp::invertImage(bool state){
	if(state){
		ofRectangle tempROI = filteredImage.getROI();
		invertBackup.setFromPixels(filteredImage.getPixels(), filteredImage.width, filteredImage.height);
		filteredImage.resetROI();
		filteredImage.invert();
		filteredImage.setROI(tempROI);
	} else {
		filteredImage.setFromPixels(invertBackup.getPixels(), invertBackup.width, invertBackup.height);
	}
}

void testApp::sharpenImage(bool state){
	if(state){
		sharpenBackup.setFromPixels(filteredImage.getPixels(), filteredImage.width, filteredImage.height);
		ofLogNotice() << "Sharpening...";
		ofxCvGrayscaleImage temp;
		temp.allocate(filteredImage.getWidth(), filteredImage.getHeight());
		temp.setFromPixels(filteredImage.getPixelsRef());
		temp.blurGaussian(sharpenBlurSize);
		ofRectangle tempROI = filteredImage.getROI();
		filteredImage.resetROI();
		filteredImage.absDiff(temp);
		filteredImage.setROI(tempROI);
	} else {
		filteredImage.setFromPixels(sharpenBackup.getPixels(), sharpenBackup.width, sharpenBackup.height);
	}
}

void testApp::guiEvent(ofxUIEventArgs &e){
    string widgetName = e.widget->getName();
    if(widgetName == "Show Filtered"){
        if(showFiltered->getValue()){
            switch(FILTER_MODE){
                case 1:
					if(grayImage.getHeight() != 0)
						boxFilter();
                    break;
            }
        }
    }
    if(widgetName == "Sharpen"){
        sharpenImage(sharpen->getValue());
    } 
	if(widgetName == "Invert"){
        invertImage(useInvert->getValue());
    } 
	if(widgetName == "Sharpen Blur"){
		sharpenImage(false);
        sharpenImage(sharpen->getValue());
    }
}
