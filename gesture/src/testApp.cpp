//Kim Ash
//3DSAV - Fall 2012
//gesture/testApp.cpp - plays applause sound when user bows
//sound source: http://www.freesound.org/people/J.Zazvurek/sounds/60789/

#include "testApp.h"

ofVec3f testApp::ConvertProjectiveToRealWorld(const XnPoint3D& p) {
	XnPoint3D world;
	depth.getXnDepthGenerator().ConvertProjectiveToRealWorld(1, &p, &world);
	return ofVec3f(world.X, world.Y, world.Z);
}

void testApp::setup() {
	ofSetVerticalSync(true);
	openni.setup();
	image.setup(&openni);
	depth.setup(&openni);
	user.setup(&openni);
	openni.registerViewport();
	openni.setMirror(true);
    applause.loadSound("applause-9s.mp3");
}

void testApp::update(){
	openni.update();
	image.update();
	depth.update();
	user.update();
    
    if(user.getNumberOfTrackedUsers() > 0) 
    {
		ofxTrackedUser* cur = user.getTrackedUser(1); // old API starts users at 1, not 0
        
        ofVec3f neckPos = ConvertProjectiveToRealWorld(cur->neck.position[1]);
        ofVec3f hipPos = ConvertProjectiveToRealWorld(cur->hip.position[1]);

        if (neckPos.y <= hipPos.y) {    //bow gesture
            applause.play();
        }
        //alternatively, use distance between neck and hip?
        //distance = neckPos.distance(hipPos);
        //use angle between torso and legs - find distance btwn hip/foot
	}
    
    applause.stop();
}

void testApp::draw(){
	ofBackground(0);
	ofSetColor(255);
	ofScale(.75, .75, .75);
	depth.draw(0, 0, 640, 480);
	image.draw(640, 0, 640, 480);
	user.draw();
}