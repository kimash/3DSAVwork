#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
    void forepoint(ofMesh mesh);
	
	ofxKinect kinect;
	ofEasyCam easyCam;
	bool useProjective;
    
    ofVec3f forePt;
    ofMesh mesh;
};
