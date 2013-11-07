#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    setupUI();
    ofSetLogLevel(OF_LOG_VERBOSE);
    decompressedImage = compressor.readDepthFrametoImage("capture/capture.raw");
    grayImage.allocate(640, 480);
    previewImage.allocate(10,10);
    grayImage.setFromPixels(decompressedImage.getPixelsRef());
    countZero();
    zeroFound->setLabel(ofToString(zeroCounter) + " zeros found.");
    selectedX = selectedY = 100;
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
    gui2->addWidgetDown(stat);
    gui2->addSpacer();
    gui2->addWidgetDown(zeroFound);
    gui2->addWidgetDown(selectedPixel);
    gui2->addSpacer();
    gui2->addWidgetDown(pixelLocation);
    gui2->addWidgetDown(pixelValue);
    gui2->addSpacer();
    up = new ofxUILabelButton(44,false,"up");
    down = new ofxUILabelButton(44,false,"down");
    left = new ofxUILabelButton(44,false,"left");
    right = new ofxUILabelButton(44,false,"right");
    gui2->addWidgetDown(up);
    gui2->addWidgetRight(down);
    gui2->addWidgetRight(right);
    gui2->addWidgetRight(left);
    ofAddListener(gui2->newGUIEvent, this, &testApp::guiEvent);
    gui1->loadSettings("GUI/guiSettings_1.xml");
	gui2->loadSettings("GUI/guiSettings_2.xml");
}

void testApp::countZero() {
    for(int i = 0; i< 640*480; i++) {
        if(grayImage.getPixels()[i] == 0) {
			zeroCounter++;
		}
    }
}

//--------------------------------------------------------------
void testApp::update(){
    
}

void testApp::setValue() {
    pixelLocation->setLabel("(" + ofToString(selectedX) + ", " + ofToString(selectedY) + ")");
    pixelValue->setLabel("Value: " + ofToString((int)grayImage.getPixels()[selectedY * 640 + selectedX]));
    //previewImage.
}

//--------------------------------------------------------------
void testApp::draw(){
    grayImage.draw(20, 70);
    ofDrawBitmapStringHighlight("x: " + ofToString(mouseX) + " y: " + ofToString(mouseY), 20, 680, ofColor::seaGreen, ofColor::white);
    ofSetColor(ofColor::red);
    ofRect(selectedX + 20, selectedY + 70, 1, 1);
    ofLine(selectedX + 20, selectedY + 70, selectedX + 40, selectedY + 40);
    ofDrawBitmapString("candidate", selectedX + 40, selectedY + 40);
    ofSetColor(ofColor::white);
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
    gui2->saveSettings("GUI/guiSettings_1.xml");
	gui2->saveSettings("GUI/guiSettings_2.xml");
    delete gui1;
	delete gui2;
}

void testApp::guiEvent(ofxUIEventArgs &e){
    string widgetName = e.widget->getName();
    if(((ofxUILabelButton*) e.widget)->getValue()) {
        if(widgetName == "up") {
            ofLogNotice() << "moving pixel up";
            selectedY--;
            if(selectedY < 0)
                selectedY = 0;
        }
        if(widgetName == "down") {
            ofLogNotice() << "moving pixel down";
            selectedY++;
            if(selectedY > 479)
                selectedY = 479;
        }
        if(widgetName == "left") {
            ofLogNotice() << "moving pixel left";
            selectedX--;
            if(selectedX < 0)
                selectedX = 0;
        }
        if(widgetName == "right") {
            ofLogNotice() << "moving pixel right";
            selectedX++;
            if(selectedX > 639)
                selectedX = 639;
        }
        setValue();
    }
}
