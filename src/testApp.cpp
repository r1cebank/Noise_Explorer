#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetBackgroundColor(179, 242, 255);
    setupUI();
	ofSetLogLevel(OF_LOG_VERBOSE);
    decompressedImage = compressor.readDepthFrametoImage("capture/capture.raw");
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
    filterOn = false;

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
    slider = new ofxUIIntSlider("Bound Size", 1, 40, &boundSize, 170, 20);
    gui2->addWidgetDown(slider);
    gui2->addWidgetDown(showFiltered);
	gui2->addWidgetDown(sharpen);
    ofAddListener(gui2->newGUIEvent, this, &testApp::guiEvent);
    gui1->loadSettings("GUI/guiSettings_1.xml");
	gui2->loadSettings("GUI/guiSettings_2.xml");
}

void testApp::countZero() {
	zeroCounter = 0;
    for(int i = 0; i< 640*480; i++) {
        if(grayImage.getPixels()[i] == 0) {
			zeroCounter++;
		}
    }
}

//--------------------------------------------------------------
void testApp::update(){
    setValue();

}

void testApp::boxFilter(){
    filteredImage.setFromPixels(grayImage.getPixelsRef());
    unsigned char *original = grayImage.getPixels();
    unsigned char *filtered = filteredImage.getPixels();
    int filterX = 0, filterY = 0;
    for(int i = 0; i < 640*480; i++){
        if(original[i] == 0){
            //Set ROI, get roi image, get average and update
            //filtered[i] = 0;
            filterY = i / 640;
            filterX = i % 640;
            filteredImage.resetROI();
            filteredImage.setROI(filterX - boundSize, filterY - boundSize, boundSize*2+1, boundSize*2+1);
            filterPreview.setFromPixels(filteredImage.getRoiPixelsRef());
            averageSelected = getAverageFromImage(filterPreview.getPixels(), pow((double)((boundSize * 2)+ 1), 2));
            filtered[i] = averageSelected;
        }
            //Find the zeros in image and replace with the average.
    }
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
		}
	}
	if(nonZero != 0)
		return (unsigned char)(totalValue / nonZero);
	else
		return 0;
}

//--------------------------------------------------------------
void testApp::draw(){
    if(!filterOn){
        grayImage.draw(20, 70);
	}
    else
        filteredImage.draw(20, 70);
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
        ofLogNotice() << "Mouse Selected at Pixel";
        selectedX = mouseX - 20;
        selectedY = mouseY - 70;
        setValue();
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if((mouseX >= 20 && mouseY >= 70) && (mouseX < 20 + 640 && mouseY < 70 + 480)) {
        ofLogNotice() << "Mouse Selected at Pixel";
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
    gui1->saveSettings("GUI/guiSettings_1.xml");
	gui2->saveSettings("GUI/guiSettings_2.xml");
    delete gui1;
	delete gui2;
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
        if(sharpen->getValue()){
			ofLogNotice() << "Sharpening...";
			ofxCvGrayscaleImage temp;
			temp.allocate(grayImage.getWidth(), grayImage.getHeight());
			temp.setFromPixels(grayImage.getPixelsRef());
			temp.blurGaussian();
			ofRectangle tempROI = grayImage.getROI();
			grayImage.resetROI();
			grayImage.absDiff(temp);
			grayImage.setROI(tempROI);
        }
    } 
}
