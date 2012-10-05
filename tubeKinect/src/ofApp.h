#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
    
    ofxOpenNIContext openni;
	ofxDepthGenerator depth;
	ofxImageGenerator image;
	ofxHandGenerator hands;
    
    ofMesh buildTube(const ofPolyline& path, int sides, float radius);
		
	int lastX, lastY;
	ofMesh mesh;
    ofPolyline line;
    
    ofEasyCam easycam;
};