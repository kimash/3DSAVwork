//Kim Ash
//3DSAV - Fall 2012
//gesture/testApp.h
//sound source: http://www.freesound.org/people/J.Zazvurek/sounds/60789/

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
    
    ofVec3f ConvertProjectiveToRealWorld(const XnPoint3D& p);
	
	ofxOpenNIContext openni;
	ofxDepthGenerator depth;
	ofxImageGenerator image;
	ofxUserGenerator user;
    ofSoundPlayer applause;
};
