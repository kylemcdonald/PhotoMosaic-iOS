#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetDataPathRoot(ofxiOSGetDocumentsDirectory());
//    ofSetOrientation(OFXIOS_ORIENTATION_LANDSCAPE_LEFT);
    NetLog::setup("control.kylemcdonald.net/log");
    
    cam.setDeviceID(1); // front camera
    cam.setup(640, 480);
    NetLog() << "camera: " << cam.getWidth() << "x" << cam.getHeight();
    
    int w = ofGetWidth();
    int h = ofGetHeight();
    NetLog() << "screen: " << w << "x" << h;
    
    string fn = "out.png";
    if(ofFile(fn).exists()) {
        img.load(fn);
    } else {
        img.allocate(w, h, OF_IMAGE_GRAYSCALE);
        for(int y = 0; y < h; y++) {
            for(int x = 0; x < w; x++) {
                bool on = x % 2 == 0 && y % 2 == 0;
                img.setColor(x, y, on ? ofColor::white : ofColor::black);
            }
        }
        img.update();
        img.save(fn);
    }
    
    
    ofxiOSDeviceInfo info = ofxiOSGetDeviceInfo();
    NetLog() << "device: " << info.deviceString;
    NetLog() << "version: " << ofxiOSNSStringToString([[UIDevice currentDevice] systemVersion]);
    NetLog() << "name: " << ofxiOSNSStringToString([[UIDevice currentDevice] name]);
    
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    float w = ofGetWidth();
    float h = ofGetHeight();
    ofSetupScreenOrtho(w, h, -1, +1);
    
//    img.draw(0, 0, w, h);
    cam.draw(w/2, h/2);
    
    ofDrawBitmapString(ofToString(round(ofGetFrameRate())), w/2, h/2);
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
//    NetLog() << "touch: " << touch.x << "," << touch.y;
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}
